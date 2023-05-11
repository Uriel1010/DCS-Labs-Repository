import serial as ser

def menu():
    print("Menu\n"
          "1. Blink RGB LED, color by color with delay of X[ms]\n"
          "2. Count up onto LCD screen with delay of X[ms]\n"
          "3. Count down onto LCD screen with delay of X[ms]\n"
          "4. Get delay time X[ms]:\n"
          "5. Potentiometer 3-digit value [v]\n"
          "6. Clear LCD screen\n"
          "7. Show menu\n"
          "8. Sleep")

def main():
    s = ser.Serial('COM5', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)   # timeout of 1 sec so that the read and write operations are blocking,
                                # after the timeout the program continues
    enableTX = True
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()
    menu()
    while (1):
        inChar = input("Enter char:")
        bytesChar = bytes(inChar, 'ascii')
        s.write(bytesChar)
        if inChar == '4':
            inChar = input("Enter time in ms: ")
            inChar = inChar[::-1] + "\n"
            bytesChar = bytes(inChar, 'ascii')
            s.write(bytesChar)
        elif inChar == '7':
            menu()

if __name__ == "__main__":
    main()