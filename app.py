def main():
    driver = open('/proc/basic_driver')
    message_from_kernel_space = driver.readline()
    print(message_from_kernel_space)
    return 


main()

