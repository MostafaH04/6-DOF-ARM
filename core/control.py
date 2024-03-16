import serial

uart = serial.Serial(port="COM2", baudrate=9600, timeout=0.1)

while True:
  coords = input("Enter the Desired xyz coordinates: ")

  uart.write()


