using Plugin.BluetoothLE;
using Plugin.BluetoothLE.Server;
using System;
using System.Diagnostics;
using System.Reactive.Linq;
using System.Text;
using System.Threading;

namespace RingSponsible.Models
{
    public class BluetoothBase
    {
        private Plugin.BluetoothLE.Server.IGattCharacteristic notifyCharacteristic = null;

        private Delegate BluetoothRecHandeler;

        private ThreadStart bluetoothRecThreadDelegate;
        private Thread recThread;

        private Adapter mAdapter = null;

        private int NUM_BYTES_IN_MESSAGE_TYPE = sizeof(BluetoothMessageTypes);

        public BluetoothBase(Delegate handeler)
        {
            Get_Default_Adapter();
            Start_Server();
        }

        public void Send_Message(BluetoothMessageTypes mess, byte[] data = null)
        {
            int datLen = 0;

            if (data != null)
            {
                datLen = data.Length;
            }

            byte[] broadcast = new byte[datLen + NUM_BYTES_IN_MESSAGE_TYPE];
            byte[] messBytes = Mess_To_Byte(mess);

            for (int i = 0; i < NUM_BYTES_IN_MESSAGE_TYPE; ++i)
            {
                broadcast[i] = messBytes[i];
            }

            for (int i = 0; i < datLen; ++i)
            {
                broadcast[i + NUM_BYTES_IN_MESSAGE_TYPE] = messBytes[i];
            }

            if (notifyCharacteristic != null)
            {
                notifyCharacteristic.Broadcast(broadcast);
            }
        }

        private async void Start_Server()
        {
            var server = CrossBleAdapter.Current.CreateGattServer();
            var service = server.AddService(Guid.NewGuid(), true);

            var characteristic = service.AddCharacteristic(
                Guid.Parse(Constants.GUID_SEND),
                CharacteristicProperties.Read | CharacteristicProperties.Write | CharacteristicProperties.WriteNoResponse,
                GattPermissions.Read | GattPermissions.Write
            );

            notifyCharacteristic = service.AddCharacteristic
            (
                Guid.Parse(Constants.GUID_NOTIFY),
                CharacteristicProperties.Indicate | CharacteristicProperties.Notify,
                GattPermissions.Read | GattPermissions.Write
            );

            IDisposable notifyBroadcast = null;
            notifyCharacteristic.WhenDeviceSubscriptionChanged().Subscribe(e =>
            {
                var @event = e.IsSubscribed ? "Subscribed" : "Unsubcribed";

                if (notifyBroadcast == null)
                {
                    notifyBroadcast = Observable
                        .Interval(TimeSpan.FromSeconds(1))
                        .Where(x => notifyCharacteristic.SubscribedDevices.Count > 0)
                        .Subscribe(_ =>
                        {
                            Debug.WriteLine("Sending Broadcast");
                            var dt = DateTime.Now.ToString("g");
                            var bytes = Encoding.UTF8.GetBytes(dt);
                            notifyCharacteristic.Broadcast(bytes);
                        });
                }
            });

            characteristic.WhenReadReceived().Subscribe(x =>
            {
                // you must set a reply value
                x.Value = Mess_To_Byte(BluetoothMessageTypes.ACKNOWLEDGE);

                BluetoothMessageTypes mess = Byte_To_Mess(x.Value);
                byte[] data = null;

                if (x.Value.Length >= NUM_BYTES_IN_MESSAGE_TYPE)
                {
                    data = new byte[x.Value.Length - NUM_BYTES_IN_MESSAGE_TYPE];

                    for (int i = 0; i < data.Length; ++i)
                    {
                        data[i] = x.Value[i + NUM_BYTES_IN_MESSAGE_TYPE];
                    }
                }

                BluetoothEventArgs e = new BluetoothEventArgs(mess, data);
                BluetoothRecHandeler.DynamicInvoke(e);

                x.Status = GattStatus.Success; // you can optionally set a status, but it defaults to Success
            });
            characteristic.WhenWriteReceived().Subscribe(x =>
            {
                var write = Encoding.UTF8.GetString(x.Value, 0, x.Value.Length);
                // do something value
            });

            await server.Start(new Plugin.BluetoothLE.Server.AdvertisementData
            {
                LocalName = "TestServer"
            });

            var scanner = CrossBleAdapter.Current.Scan().Subscribe(scanResult =>
            {
                // do something with it
                // the scanresult contains the device, RSSI, and advertisement packet

            });

            scanner.Dispose(); // to stop scanning
        }

        private async void Get_Default_Adapter()
        {
            int i = 0;
        }

        private byte[] Int_To_Byte(Int16 intValue)
        {
            byte[] intBytes = BitConverter.GetBytes(intValue);
            return intBytes;
        }
        private byte[] Mess_To_Byte(BluetoothMessageTypes message)
        {
            return Int_To_Byte((Int16)message);
        }

        private Int16 Byte_To_Int(byte[] intBytes)
        {
            Int16 intVal = BitConverter.ToInt16(intBytes, 0);
            return intVal;
        }
        private BluetoothMessageTypes Byte_To_Mess(byte[] messBytes)
        {
            return (BluetoothMessageTypes)Byte_To_Int(messBytes);
        }
    }
}