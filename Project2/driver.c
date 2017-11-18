/*
 * 2017, İTÜ. System Programming Course
 *  
 * Project 2
 * 
 * 150140119 - Muhammed Kadir Yücel
 * 150140123 - Mahmut Lutfullah Özbilen
 * 
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/cred.h>
#include <linux/uidgid.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/file.h> 
#include <linux/buffer_head.h>

#include <asm/switch_to.h>
#include <asm/uaccess.h>
#include <asm/segment.h>

#include "driver_ioctl.h"

#define DEVICE_MAJOR 0
#define READ_MODE 0
#define DEVICE_NR_DEVS 4
#define UNREAD_LIMIT 100
#define BUF_SIZE 4096

int device_major = DEVICE_MAJOR;
int device_minor = 0;
int read_mode = READ_MODE; // 0 = Just unread, 1 = include read
int device_nr_devs = DEVICE_NR_DEVS; // number of devices
int unread_limit = UNREAD_LIMIT; // unread limit for one user

module_param(unread_limit, int, S_IRUGO); // unread_limit as module parameter

MODULE_AUTHOR("Muhammed Kadir Yucel, Mahmut Lutfullah Ozbilen");
MODULE_LICENSE("ITU/ce");

struct user_message { // linked list struct to hold user message
	int fromUser; // user id of message sender
	int toUser; // user id of message receiver
	char *messageContent; // content of message
	size_t length; // length of the messsage
	int read; // if message is read
	struct user_message *next; // linked list!
};

struct device_dev { // struct for device
	struct user_message *head; // head of the linked list
	struct semaphore sem; // we need to lock the device 
	struct cdev cdev; // character device :)
};

struct device_dev *devices; // devices array
struct user_message *temp_message; // temp message pointer for device_read operation

char* int_to_char (int uid, int* size){ // convert interger to char
	int tmp,i;
	char *c;
	if(uid == 0){
		(*size) = 1;
		c = kmalloc(1*sizeof(char), GFP_KERNEL );
		c[0] = '0';
		return c;
	}
	tmp = uid;
	while(tmp > 0){ 
		tmp = tmp/10;
		(*size)++;
	}
	c = kmalloc((*size)*sizeof(char), GFP_KERNEL );
	tmp = uid;
	i = (*size);
	i--;
	while(uid > 0){ 
		tmp = uid%10;
		c[i] = (char)(((int)'0')+tmp);
		uid = uid/10;
		i--;
	}
	return c;
}

int username_to_userid (char* username, int username_length){ // convert username to id
	struct file *passwdFile = NULL;
	
	mm_segment_t old_fs;
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	passwdFile = filp_open("/etc/passwd",O_RDONLY,0);
	set_fs(old_fs);
	
	if(passwdFile == NULL){
		printk(KERN_ALERT "Cannot open file\n");
		return -1;
	}
	
	char buffer[BUF_SIZE];
	int i;
	for(i=0;i<BUF_SIZE;i++)
		buffer[i] = 0;
	loff_t position = 0;

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	vfs_read(passwdFile,buffer,BUF_SIZE,&position);
	set_fs(old_fs);
	filp_close(passwdFile,0);
	
	int found = 0;
	int index = 0;
	
	while(index < BUF_SIZE && !found){
		for(i = 0; i < username_length; i++){
			found = 1;
			if(username[i] != buffer[index + i]){
					found = 0;
					break;
			}
		}
		if(!found){
			while(buffer[index] != '\n' && index < BUF_SIZE)
				index++;
			if(index < BUF_SIZE)
				index++;
		}
	}
	if(found){
		index = index + username_length + 3;
		int uid = 0;
		for(i = index; buffer[i] != ':'; i++){
			uid = 10 * uid + buffer[i] - '0';
		}
		printk(KERN_INFO "uid is %d\n", uid);
		return uid; 
	}
	else{
		printk(KERN_ALERT "User cannot found\n");
		return -1;
	}
}

char* userid_to_username(char *uid, int* size, int uidSize){
	struct file *passwdFile = NULL;
	
	mm_segment_t old_fs;
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	passwdFile = filp_open("/etc/passwd",O_RDONLY,0);
	set_fs(old_fs);
	
	if(passwdFile == NULL){
		printk(KERN_ALERT "Cannot open file\n");
		return -1;
	}
	
	char buffer[BUF_SIZE];
	int i;
	for(i=0;i<BUF_SIZE;i++)
		buffer[i] = 0;
	loff_t position = 0;

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	vfs_read(passwdFile,buffer,BUF_SIZE,&position);
	set_fs(old_fs);
	filp_close(passwdFile,0);
	
	int found = 0;
	int index = 0;
	int colonCounter = 0;
	int lineCounter = 0;
	int usernameSize = 0;
	printk(KERN_INFO "Incoming uid is %s", uid);
	while(index < BUF_SIZE && !found){
		while(colonCounter < 2){
			if(buffer[index] == ':')
				colonCounter++;
			if(colonCounter == 1){
				usernameSize = lineCounter-1;
			}
			index++;
			lineCounter++;
		}
		for(i = 0; i < uidSize; i++){
			found = 1;
			if(uid[i] != buffer[index + i]){
					found = 0;
					break;
			}
		}
		if(!found){
			while(buffer[index] != '\n' && index < BUF_SIZE){
				index++;
				lineCounter++;
			}
			if(index < BUF_SIZE)
				index++;
			lineCounter = 0;
			colonCounter = 0;
		}
	}
	if(found){
		index = index - lineCounter;
		(*size) = usernameSize;
		char* username = kmalloc(usernameSize * sizeof(char), GFP_KERNEL);
		for(i = 0; i < usernameSize; i++){
			username[i] = buffer[index + i];
		}
		printk(KERN_INFO "username size is %d\n", usernameSize);
		printk(KERN_INFO "username is %s\n", username);
		return username; 
	}
	else{
		printk(KERN_ALERT "User cannot found\n");
		return NULL;
	}
}

int device_trim(struct device_dev *dev){ // free-up memory
	printk(KERN_INFO "Trim region\n");
	
	struct user_message *temp = dev->head;
	struct user_message *toDelete;
	while(temp != NULL){
		toDelete = temp;
		temp = temp->next;
		kfree(toDelete->messageContent);
		kfree(toDelete);
	}
	printk(KERN_INFO "Trim region exit\n");
	return 0;
}

int device_open(struct inode *inode, struct file *filp){ // open device
	printk(KERN_INFO "Open region\n");
	struct device_dev *dev;
	dev = container_of(inode->i_cdev, struct device_dev, cdev);
	filp->private_data = dev;
	
	if(temp_message == NULL)
		temp_message = dev->head;
	
	return 0;
}

int device_release(struct inode *inode, struct file *filp){ // release device
	printk(KERN_INFO "Release region\n");
	return 0;
}

ssize_t device_read(struct file *filp, char __user *buf, size_t count,
					loff_t *f_pos)
{
	printk(KERN_INFO "Read region\n");
	struct device_dev *dev = filp->private_data;
	size_t return_size = 0;
	
	if(down_interruptible(&dev->sem)) // semaphore check
		return -ERESTARTSYS;
	
	printk(KERN_INFO "Got semaphore\n");
	if(dev->head == NULL){
		printk("No message in device!!\n");
		up(&dev->sem);
		return 0;
	}
	
	int user_id = get_current_user()->uid.val;
	while(return_size == 0){
		if(temp_message == NULL){
			printk("Temp message is null\n");
			temp_message = dev->head;
			break;
		}
		if(temp_message->toUser == user_id){ // if message belongs to user
			if(read_mode == 0 && temp_message->read == 1){ // EXCLUDE_READ and read message
				printk("Read message is only unread mode\n");
				temp_message = temp_message->next;
				continue;
			}
			int user_id_length = 0;
			int username_length = 0;
			printk("Converting id to char\n");
			char* user_id = int_to_char(temp_message->fromUser, &user_id_length);
			printk("Id in char %s\n", user_id);
			char* username = userid_to_username(user_id, &username_length, user_id_length);
			if(username == NULL){
				up(&dev->sem);
				return -EINVAL;
			}
			int return_msg_length = username_length + temp_message->length + 3; // id length, message length and ': ...\0'
			char* return_msg = kmalloc(return_msg_length * sizeof(char), GFP_KERNEL);
			int i;
			for(i = 0; i < username_length; i++){
				return_msg[i] = username[i];
			}
			return_msg[i++] = ':';
			return_msg[i++] = ' ';
			printk("i value is %d\n", i);
			int j = 0;
			while(i < return_msg_length){
				return_msg[i++] = temp_message->messageContent[j++];
			}
			return_msg[return_msg_length - 2] = '\n';
			return_msg[return_msg_length - 1] = '\0';
			
			return_size = return_msg_length;
			if(copy_to_user(buf, return_msg, return_msg_length)){ // send to user
				up(&dev->sem);
				return -EFAULT;
			}
			printk("Message printed on users screen\n");
			temp_message->read = 1;
		}
		temp_message = temp_message->next;
	}
	up(&dev->sem);
	printk(KERN_INFO "Read region end with %d\n", return_size);
	return return_size;
}

ssize_t device_write(struct file *filp, const char __user *buf, size_t count,
						loff_t *f_pos)
{
		printk(KERN_INFO "Write region\n");
		struct device_dev *dev = filp->private_data;
		ssize_t ret_val = -ENOMEM;
		
		if(down_interruptible(&dev->sem))
			return -ERESTARTSYS;
		
		struct user_message *incomeMessage;
		incomeMessage = kmalloc(sizeof(struct user_message), GFP_KERNEL);
		if(!incomeMessage){
			up(&dev->sem);
			return -ENOMEM;
		}
		printk("Income message created\n");
		
		int user_id = get_current_user()->uid.val;
		incomeMessage->fromUser = user_id;
		incomeMessage->length = count;
		incomeMessage->next = NULL;
		printk("Income message length is %d\n", incomeMessage->length);
		incomeMessage->messageContent = kmalloc(incomeMessage->length*sizeof(char), GFP_KERNEL);
		if(!incomeMessage->messageContent){
			up(&dev->sem);
			return -ENOMEM;
		}
		printk("Income message content created\n");
		
		if(copy_from_user(incomeMessage->messageContent, buf, count)){
			up(&dev->sem);
			return -EFAULT;
		}
		incomeMessage->messageContent[incomeMessage->length-1] = '\0';
		printk("Income message copied from user %s\n", incomeMessage->messageContent);
		
		char* dest_user;
		if(*(incomeMessage->messageContent) == '@'){
			dest_user = incomeMessage->messageContent + 1;
		}else {
			up(&dev->sem);
			return -EINVAL;
		}
		
		char* message;
		message = dest_user;
		char* dest_username;
		int username_length = 0;
		while(*message != ' '){
			message++;
			username_length++;
		}
		
		if(username_length >= 32){
			up(&dev->sem);
			return -EINVAL;
		}
		dest_username = kmalloc(username_length * sizeof(char), GFP_KERNEL);
		if(!dest_username){
			up(&dev->sem);
			return -ENOMEM;
		}
		
		int i;
		message = dest_user;
		for(i = 0; i < username_length; i++){
			dest_username[i] = *message;
			message++;
		}
		printk(KERN_INFO "Username is %s and username length is %d", dest_username,username_length);
		
		incomeMessage->toUser = username_to_userid(dest_username, username_length);
		if(incomeMessage->toUser == -1){
			up(&dev->sem);
			return -EINVAL;
		}
		printk("Id converted from username %d\n", incomeMessage->toUser);
		
		// check for unread messages count for user
		struct user_message *unread_check = dev->head;
		int unread_counter = 0;
		while(unread_check != NULL){
			if(unread_check->toUser == incomeMessage->toUser && unread_check->read == 0)
				unread_counter++;
				
			unread_check = unread_check->next;
		}
		if(unread_counter >= unread_limit){
			printk("Too many unread messages for user %d\n", incomeMessage->toUser);
			kfree(incomeMessage);
			up(&dev->sem);
			return -ENOSPC;
		}		
		
		// message is at ' '
		message++;
		incomeMessage->length = count - username_length - 2;
		printk("income message length %d count %d username_length %d\n", incomeMessage->length, count, username_length);
		incomeMessage->read = 0;
		char* temp_message = kmalloc(incomeMessage->length * sizeof(char), GFP_KERNEL);
		if(!temp_message){
			up(&dev->sem);
			return -ENOMEM;
		}
		int a;
		for(a =0; a < incomeMessage->length; a++){
			temp_message[a] = message[a];
		}
		printk("Message content is %s\n", temp_message);
		
		if(incomeMessage->messageContent != NULL)
			kfree(incomeMessage->messageContent);
			
		incomeMessage->messageContent = temp_message;
		struct user_message *temporary = dev->head;
		incomeMessage->next = NULL;
		
		if(dev->head == NULL)
			dev->head = incomeMessage;
		else{		
			while(temporary->next != NULL)
				temporary = temporary->next;
				
			temporary->next = incomeMessage;
		}

		temp_message = dev->head;
			
		up(&dev->sem);
		return count;
}

long device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_INFO "IOCTL Region\n");
	if(!capable(CAP_SYS_ADMIN))
		return -EPERM;

	int user_id;
	switch(cmd){
		case SET_READ_MODE:
			read_mode = (int) arg;
			printk("GOT READ_MODE %d\n", read_mode);
			break;
		case SET_UNREAD_LIMIT:
			unread_limit = (int) arg;
			printk("GOT UNREAD_LIMIT %d\n", unread_limit);
			break;
		case CLEAR_MESSAGE_OF:			
			user_id = (int) arg;
			printk("GOT USER_ID %d\n", user_id);
			
			struct device_dev *dev = filp->private_data;
			
			if(down_interruptible(&dev->sem))
				return -ERESTARTSYS;
				
			struct user_message *del_temp = dev->head;
			struct user_message *prev;
			
			while(del_temp != NULL && del_temp->toUser == user_id){
				dev->head = del_temp->next;
				if(del_temp->messageContent != NULL)
					kfree(del_temp->messageContent);
				kfree(del_temp);
				del_temp = dev->head;
			}
			
			while(del_temp != NULL){
				while(del_temp != NULL && del_temp->toUser != user_id){
					prev = del_temp;
					del_temp = del_temp->next;
				}
				
				if(del_temp == NULL){
					up(&dev->sem);
					 return 0;
				}
				prev->next = del_temp->next;
				if(del_temp->messageContent != NULL)
					kfree(del_temp->messageContent);
				kfree(del_temp);
				
				del_temp = prev->next;
			}
			up(&dev->sem);
			
			break;
		default:
			return -EINVAL;
	}
	
	printk(KERN_INFO "IOCTL Region exit\n");
	return 0;
}
	
struct file_operations device_fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release,
};

void device_exit_module(void)
{
	printk(KERN_INFO "Module exit\n");
	
	dev_t dev_no = MKDEV(device_major, device_minor);
	int i;
	if(devices){
		printk(KERN_INFO "Cleaning devices\n");
		for(i = 0; i < device_nr_devs; i++){
			device_trim(devices + i); // clean memory
			cdev_del(&devices[i].cdev);
		}
		kfree(devices); // remove devices array
	}
	
	unregister_chrdev_region(dev_no, device_nr_devs); // free numbers from system
}

int device_init_module(void)
{
	printk(KERN_INFO "Module initialization\n");
	int ret_val;
	dev_t dev_no = 0;
	
	if(device_major){ // get device major numbers
		printk(KERN_INFO "Register region\n");
		dev_no = MKDEV(device_major, device_minor);
		ret_val = register_chrdev_region(dev_no, device_nr_devs, "mydevice");
	}else{
		printk(KERN_INFO "Alloc region\n");
		ret_val = alloc_chrdev_region(&dev_no, device_minor, device_nr_devs, "mydevice");
		device_major = MAJOR(dev_no);
	}
	
	if(ret_val < 0){
		printk(KERN_WARNING "mydevice: cannot get major %d\n", device_major);
		return ret_val;
	}
	
	printk(KERN_INFO "Set device\n");
	// set for main device
	
	printk(KERN_INFO "Device initializations\n");
	devices = kmalloc(device_nr_devs * sizeof(struct device_dev), GFP_KERNEL);
	if(!devices)
		return -ENOMEM;
		
	memset(devices, 0, device_nr_devs * sizeof(struct device_dev));
	struct device_dev *dev;
	int i;
	int error;
	for(i = 0; i < device_nr_devs; i++){ // create nr_devices
		dev = &devices[i];
		sema_init(&dev->sem, 1);
		dev_no = MKDEV(device_major, device_minor + i);
		dev->head = NULL;
		cdev_init(&dev->cdev, &device_fops);
		dev->cdev.owner = THIS_MODULE;
		dev->cdev.ops = &device_fops;
		error = cdev_add(&dev->cdev, dev_no, 1);
		if(error)
			printk(KERN_WARNING "Error %d adding mydevice%d", error, i);
	}
		
	return 0;
}

module_init(device_init_module);
module_exit(device_exit_module);
