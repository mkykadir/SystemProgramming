cmd_drivers/acpi/acpi.o := ld -m elf_i386   -r -o drivers/acpi/acpi.o drivers/acpi/osl.o drivers/acpi/utils.o drivers/acpi/reboot.o drivers/acpi/nvs.o drivers/acpi/wakeup.o drivers/acpi/sleep.o drivers/acpi/device_pm.o drivers/acpi/proc.o drivers/acpi/bus.o drivers/acpi/glue.o drivers/acpi/scan.o drivers/acpi/resource.o drivers/acpi/acpi_processor.o drivers/acpi/processor_core.o drivers/acpi/ec.o drivers/acpi/dock.o drivers/acpi/pci_root.o drivers/acpi/pci_link.o drivers/acpi/pci_irq.o drivers/acpi/acpi_platform.o drivers/acpi/power.o drivers/acpi/event.o drivers/acpi/sysfs.o drivers/acpi/acpi_cmos_rtc.o drivers/acpi/debugfs.o drivers/acpi/video_detect.o 
