using System;
using System.Collections.Generic;

namespace CacheController
{
    class CacheMemory
    {
        public List<bool> valids;
        public List<Int32> addresses;
        public List<byte> data;
        public int capacity;

        public CacheMemory(int capacity)
        {
            Console.WriteLine("INITALIZED Cache Memory: All valid bits set 0, capacity: {0}", capacity);
            this.capacity = capacity;
            valids = new List<bool>(new bool[capacity]);
            addresses = new List<Int32>(new Int32[capacity]);
            data = new List<byte>(new byte[capacity]);
        }

        public void Write(Int32 address, byte data)
        {
            byte tag = (byte)(address % capacity);
            Console.WriteLine("For address '{0}' used tag '{1}'", Convert.ToString(address, 2), tag);
            this.addresses[tag] = address;
            this.data[tag] = data;
            this.valids[tag] = true;
            Console.WriteLine("Data writed and valid bit set 1");
        }
        
        public byte? Read(Int32 address)
        {
            byte tag = (byte)(address % capacity);
            Console.WriteLine("For address '{0}' used tag '{1}'", Convert.ToString(address, 2), tag);
            if (!valids[tag])
                return null;
            return this.data[tag];
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            var RAM = new Dictionary<Int32, byte>();
            bool isRead;
            Int32 addressBus;
            byte dataBus;

            var cacheMemory = new CacheMemory(8);
            while (true)
            {
                Console.Write("Enter action number (0 - read, 1 - write): ");
                isRead = Console.ReadLine() == "0";
                Console.Write("Enter address (32 bit): ");
                addressBus = Convert.ToInt32(Console.ReadLine(), 2);
                Console.WriteLine("Reading bit is {0} from busX", isRead ? 1 : 0);
                if (isRead)
                {
                    var data = cacheMemory.Read(addressBus);
                    if (data != null)
                    {
                        Console.WriteLine("HIT! Data: " + Convert.ToString((byte)data, 2));
                        Console.WriteLine("Sending data to CPU bus D1!");
                    } else {
                        Console.WriteLine("Miss. Reading from RAM D2 and sending to CPU D1!");
                    }
                } else {
                    Console.Write("Enter data (8 bit): ");
                    dataBus = Convert.ToByte(Console.ReadLine(), 2);
                    RAM[addressBus] = dataBus;
                    Console.WriteLine("Data is successfully recorded in RAM D2!");
                    cacheMemory.Write(addressBus, dataBus);
                    Console.WriteLine("Writed HIT!");
                }
            }
        }
    }
}
