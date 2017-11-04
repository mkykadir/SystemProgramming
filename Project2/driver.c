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

#include <asm/switch_to.h>
#include <asm/uaccess.h>

#include "driver_ioctl.h"

#define DEVICE_MAJOR 0
#define READ_MODE 0
#define DEVICE_NR_DEVS 4
#define UNREAD_LIMIT 100

int device_major = DEVICE_MAJOR;
int device_minor = 0;
int read_mode = READ_MODE; // 0 = Just unread, 1 = include read
int device_nr_devs = DEVICE_NR_DEVS;
int unread_limit = UNREAD_LIMIT;
// int unread_limit = 10;

// May be module parameter!
module_param(unread_limit, int, S_IRUGO);

MODULE_AUTHOR("Muhammed Kadir Yücel, Mahmut Lutfullah Özbilen");
MODULE_LICENSE("ITU/ce");

struct user_message {
	int fromUser;
	int toUser;
	char *messageContent;
	size_t length;
	int read;
	struct user_message *next;
};

struct device_dev {
	// message struct array
	struct user_message *head;
	struct semaphore sem;
	struct cdev cdev;
};

struct device_dev *devices;
struct user_message *temp_message;

int char_2_int(char* uid, int size){
	int i,j,k,tmp_exp;
	int returnNumber = 0;
	char *tmp;
	j=0;
	for(i=size-1; i >= 0; --i){
		tmp = uid+i;
		tmp_exp = 1;

		for(k=0;k<j;k++){//c de ust ifadesi yok!
			tmp_exp = tmp_exp * 10;
		}

		returnNumber += tmp_exp * ( *tmp - 48);

		j = j+1;
	}
	
	return returnNumber;
}

char* int_2_char (int uid, int* size){
	
	int tmp,i;
	char *c;
	if(uid == 0){ //root check
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

int device_trim(struct device_dev *dev){
	printk(KERN_INFO "Trim region\n");
	return 0;
}

int device_open(struct inode *inode, struct file *filp){
	printk(KERN_INFO "Open region\n");
	struct device_dev *dev;
	dev = container_of(inode->i_cdev, struct device_dev, cdev);
	filp->private_data = dev;
	
	return 0;
}

int device_release(struct inode *inode, struct file *filp){
	printk(KERN_INFO "Release region\n");
	return 0;
}

ssize_t device_read(struct file *filp, char __user *buf, size_t count,
					loff_t *f_pos)
{
	printk(KERN_INFO "Read region\n");
	struct device_dev *dev = filp->private_data;
	size_t return_size = 0;
	
	if(down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	
	printk("Got semaphore\n");
	if(dev->head == NULL){
		// mesaj yok!
		printk("No message in device!!\n");
		up(&dev->sem);
		return 0;
	}
	
	int user_id = get_current_user()->uid.val;
	// read mode check yap
	struct user_message *temp = dev->head;
	while(return_size == 0){
		if(temp_message == NULL){
			printk("Temp message is null\n");
			temp_message = dev->head;
			break;
		}
		if(temp->toUser == user_id){
			if(read_mode == 0 && temp->read == 1){
				printk("Read message is only unread mode\n");
				temp = temp->next;
				temp_message = temp_message->next;
				continue;
			}
			int user_id_length = 0;
			printk("Converting id to char\n");
			char* user_id = int_2_char(temp->fromUser, &user_id_length);
			printk("Id in char %s\n", user_id);
			int return_msg_length = user_id_length + temp->length + 3;
			char* return_msg = kmalloc(return_msg_length * sizeof(char), GFP_KERNEL);
			int i;
			for(i = 0; i < user_id_length; i++){
				return_msg[i] = user_id[i];
			}
			return_msg[i++] = ':';
			return_msg[i++] = ' ';
			printk("i value is %d\n", i);
			int j = 0;
			while(i < return_msg_length){
				return_msg[i++] = temp->messageContent[j++];
			}
			return_msg[return_msg_length - 2] = '\n';
			return_msg[return_msg_length - 1] = '\0';
			
			return_size = return_msg_length;
			if(copy_to_user(buf, return_msg, return_msg_length)){
				up(&dev->sem);
				return -EFAULT;
			}
			printk("Message printed on users screen\n");
			temp->read = 1;
		}
		temp = temp->next;
		temp_message = temp_message->next;
	}
	up(&dev->sem);
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
		
		char* dest_username;
		char* message;
		message = dest_user;
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
		// printk("Destination username is %s\n", dest_username);
		
		// get user id from username
		// check for user id
		
		incomeMessage->toUser = char_2_int(dest_username, username_length);
		printk("Id converted to int %d\n", incomeMessage->toUser);
		
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
		printk("income message length %d count %d username_length %d", incomeMessage->length, count, username_length);
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
				
				if(del_temp == NULL) return 0;
				prev->next = del_temp->next;
				if(del_temp->messageContent != NULL)
					kfree(del_temp->messageContent);
				kfree(del_temp);
				
				del_temp = prev->next;
			}
			
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
			device_trim(devices + i);
			cdev_del(&devices[i].cdev);
		}
		kfree(devices);
	}
	
	unregister_chrdev_region(dev_no, device_nr_devs);		
}

int device_init_module(void)
{
	printk(KERN_INFO "Module initialization\n");
	int ret_val;
	dev_t dev_no = 0;
	
	if(device_major){
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
	for(i = 0; i < device_nr_devs; i++){
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
