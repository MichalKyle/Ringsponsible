package edu.illinois.echoes;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static android.R.attr.data;

public class BluetoothWriter extends edu.illinois.echoes.CustBluetooth
{


    public boolean WriteMessage(OutputStream out, int messageType) throws IOException
    {
        return WriteMessageByteArr(out, messageType, null);
    }
    public boolean WriteMessage(OutputStream out, int messageType, byte[] data) throws IOException
    {
        return WriteMessageByteArr(out, messageType,  data);
    }
    public boolean WriteMessage(OutputStream out, int messageType, int data) throws IOException
    {
        return WriteMessageByteArr(out, messageType, intToByteArray(data));
    }
    public boolean WriteMessage(OutputStream out, int messageType, int arg1, int arg2) throws IOException
    {
        byte[] bArg1 = intToByteArray(arg1);
        byte[] bArg2 = intToByteArray(arg2);

        byte[] data = new byte[bArg1.length + bArg2.length];

        for (int i = 0; i < bArg1.length; ++i)
        {
            data[i] = bArg1[i];
        }
        for (int i = 0; i < bArg1.length; ++i)
        {
            data[i + bArg1.length] = bArg2[i];
        }

        return WriteMessageByteArr(out, messageType,  data);
    }


    private boolean WriteMessageByteArr(OutputStream out, int messageType, byte[] data) throws IOException
    {
        int dataLen = 0;
        if (data != null)
        {
            dataLen = data.length;
        }

        byte[] bMessageType = intToByteArray(messageType);
        int mess_len = dataLen + bMessageType.length;
        byte[] bMessLen = intToByteArray(mess_len);

        byte[] fullMessage = new byte[mess_len + bMessLen.length];

        for (int i = 0; i < bMessLen.length; ++i)
        {
            fullMessage[i] = bMessLen[i];
        }

        for (int i = 0; i < bMessageType.length; ++i)
        {
            fullMessage[i + bMessLen.length] = bMessageType[i];
        }

        for (int i = 0; i < dataLen; ++i)
        {
            fullMessage[i + bMessLen.length + bMessageType.length] = data[i];
        }

        out.write(fullMessage);

        return true;
    }

    private byte[] intToByteArray(int i)
    {
        final ByteBuffer bb = ByteBuffer.allocate(NUM_BYTES_PER_INT);
        bb.order(ByteOrder.LITTLE_ENDIAN);
        bb.putInt(i);
        return bb.array();
    }
}
