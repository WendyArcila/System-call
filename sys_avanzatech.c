#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>


asmlinkage long avanzatech(int number, char __user *username, size_t username_length, char __user *dest_buffer, size_t dest_len){
    // Variables para almacenar datos temporales
    int result;
    char response[70];
    char *kernel_buffer;

    printk("Entra a la llamada wiii\n");
    if (username_length > 25)
    {
        printk("Valor de longitud de nombre de usuario no valido\n");
        return -EINVAL; // Valor de longitud de nombre de usuario no valido
    }
    printk("Verifica permisos de acceso y tamanos de los buferes\n");
    // Verifica permisos de acceso y tamanos de los buferes
    if (!access_ok(dest_buffer, dest_len) || !access_ok(username, username_length))
    {
        printk("Error de acceso\n");
        return -EFAULT; // Error de acceso
    }

    // Asigna memoria dinamicamente para el bufer del kernel
    kernel_buffer = kmalloc(dest_len, GFP_KERNEL);
    if (kernel_buffer == NULL)
    {
        return -ENOMEM; // Error de memoria
    }

    printk(" Copia el numero desde el espacio de usuario al espacio del kernel\n");
    // Copia el numero desde el espacio de usuario al espacio del kernel
    if (copy_from_user(kernel_buffer, username, username_length))
    {
        printk("Error de acceso\n");
        kfree(kernel_buffer);
        return -EFAULT; // Error de copia
    }
    printk("Verifica el valor max del numero\n");
    if (number < -1290 || number > 1290)
    {
        printk("Error de acceso\n");
        kfree(kernel_buffer);
        return -EINVAL; // Valor de numero no valido
    }
    // Realiza la operacion (en este caso, el cubo del numero)
    result = number * number * number;
    printk("Construye el mensaje de respuesta\n");
    // Construye el mensaje de respuesta
    snprintf(response, sizeof(response), "Hi %s, the cube of %d is %d", kernel_buffer, number, result);

    if (strlen(response) + 1 > dest_len)
    {
        printk("Error de memoria insuficiente en el bufer de destino\n");
        kfree(kernel_buffer);
        return -ENOMEM; // Error de memoria insuficiente en el bufer de destino
    }

    // Copia la respuesta al bufer de destino en el espacio de usuario
    if (copy_to_user(dest_buffer, response, strlen(response) + 1))
    {
        printk("Error de tamano de bufer insuficiente\n");
        kfree(kernel_buffer);
        return -ENOBUFS; // Error de tamano de bufer insuficiente
    }

    // Libera la memoria cuando hayas terminado de usar el bufer del kernel
    kfree(kernel_buffer);

    return 0; 
}

//cuando se definen los dos buffer en la system call, hay uno que está destinado al sistema 