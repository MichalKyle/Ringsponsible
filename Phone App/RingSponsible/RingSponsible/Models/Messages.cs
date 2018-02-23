using System;

namespace RingSponsible.Models
{
    public enum BluetoothMessageTypes : Int16
    {
        ACKNOWLEDGE,
        N_MESS_TYPES,
        NONE
    }

    public static class Constants
    {
        public const string GUID_NOTIFY = "c6446abf-22df-4547-b1ef-a7db9d3d9023";
        public const string GUID_SEND = "3fb17958-700a-40ea-91c5-1909a3a7c06a";
    }
}