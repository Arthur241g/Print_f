/*
 * This code is a simple implementation of the `printf` function.
 *
 * It takes a format string and a variable number of arguments,
 * and prints the formatted output to the standard output.
 */

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * This function prints a character to the standard output.
 */
int my_putchr(char c) {
  return write(1, &c, 1);
}

/*
 * This function prints a number in a given base.
 */
int print_base(size_t num, int base) {
  char *ch_array = (char *)malloc(sizeof(char));
  int index = 0, count = 0;
  do {
    int modulus = num % base;
    if (modulus >= 0 && modulus < 10) {
      ch_array[index] = modulus + '0';
    } else {
      ch_array[index] = 'a' + modulus - 10;
    }
    ch_array = realloc(ch_array, sizeof(char) * ++index);
    num /= base;
  } while (num > 0);
  do {
    index--;
    count += my_putchr(ch_array[index]);
  } while (index);
  free(ch_array);
  return count;
}

/*
 * This function prints a decimal number.
 */
int print_decimal(va_list ap) {
  int current = va_arg(ap, int);
  int count = 0;
  if (current < 0) {
    current = -current;
    count += my_putchr('-');
  }
  return count + print_base(current, 10);
}

/*
 * This function prints a number in a given base.
 */
int print_number(va_list ap, int base) {
  size_t current = va_arg(ap, size_t);
  return print_base(current, base);
}

/*
 * This function prints a character.
 */
int print_char(va_list ap) {
  char current = va_arg(ap, int);
  return my_putchr(current);
}

/*
 * This function prints a string.
 */
int print_string(va_list ap) {
  char *str = va_arg(ap, char *);
  if (str == NULL) {
    str = "(null)";
  }
  int count = 0;
  while (*str != '\0') {
    count += my_putchr(*str);
    str++;
  }
  return count;
}

/*
 * This function prints a pointer in hexadecimal format.
 */
int print_pointer(va_list ap) {
  void *ptr = va_arg(ap, void *);
  size_t addr = (size_t)ptr;
  return write(1, "0x", 2) + print_base(addr, 16);
}

/*
 * This function prints the formatted output according to the format string.
 */
int print_format(va_list ap, char format) {
  switch (format) {
    case 'd':
      return print_decimal(ap);
    case 'o':
      return print_number(ap, 8);
    case 'u':
      return print_number(ap, 10);
    case 'x':
      return print_number(ap, 16);
    case 'c':
      return print_char(ap);
    case 's':
      return print_string(ap);
    case 'p':
      return print_pointer(ap);
    default:
      return 0;
  }
}

/*
 * This function is the main entry point of the program.
 */
int my_printf(char *restrict format, ...) {
  int total_count = 0;
  va_list ap;
  va_start(ap, format);
  while (*format != '\0') {
    if (*format == '%' && *(format + 1) != '\0') {
      /*
       * This is the case where the format string contains a format specifier.
       * We call the `print_format` function to print the formatted output.
       */
      format++;
      total_count += print_format(ap, *format);
    } else {
      /*
       * This is the case where the format string does not contain a format specifier.
       * We simply print the character as is.
       */
      total_count += my_putchr(*format);
    }
    format++;
  }
  va_end(ap);
  return total_count;
}