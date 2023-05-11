import serial as ser
import time

def menu():
    print("Menu\n"
          "1. Blink RGB LED, color by color with delay of X[ms]\n"
          "2. Count up onto LCD screen with delay of X[ms]\n"
          "3. Count down onto LCD screen with delay of X[ms]\n"
          "4. Get delay time X[ms]:\n"
          "5. Potentiometer 3-digit value [v]\n"
          "6. Clear LCD screen\n"
          "7. Show menu\n"
          "8. Sleep\n"
          "9. On each PB1 pressed, Send a Massage “I love my Negev”\n")

def main():
    s = ser.Serial('COM3', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)   # timeout of 1 sec so that the read and write operations are blocking,
                                # after the timeout the program continues
    flag_9 = False
    enableTX = True
    start = 0
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()
    menu()
    while (1):
        while (s.in_waiting > 0):  # while the input buffer isn't empty
            line = s.read_until()  # read  from the buffer until the terminator is received,
            # readline() can also be used if the terminator is '\n'
            print(line.decode("ascii"))
            start = time.time()
        if (s.in_waiting == 0 and flag_9 == False):
            enableTX = True
        while (s.out_waiting > 0 or enableTX):
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
            elif inChar == '9':
                start = time.time()
                flag_9 = True
            if s.out_waiting == 0:
                enableTX = False
        if time.time() - start > 5 and flag_9 == True:
            flag_9 = False



if __name__ == "__main__":
    main()