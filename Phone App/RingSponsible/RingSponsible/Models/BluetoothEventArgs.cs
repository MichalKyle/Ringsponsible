using System;

namespace RingSponsible.Models
{
    public class BluetoothEventArgs
    {
        public byte[] recievedData;
        public BluetoothMessageTypes mess;

        public BluetoothEventArgs()
        {
            recievedData = null;
            mess = BluetoothMessageTypes.N_MESS_TYPES;
        }

        public BluetoothEventArgs(BluetoothMessageTypes initMess)
        {
            recievedData = null;
            mess = initMess;
        }

        public BluetoothEventArgs(BluetoothMessageTypes initMess, byte[] payload)
        {
            recievedData = payload;
            mess = initMess;
        }
    }
}