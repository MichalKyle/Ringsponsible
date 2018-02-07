package edu.illinois.echoes;

import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static java.lang.System.out;

public class BluetoothReader extends edu.illinois.echoes.CustBluetooth
{
    private String LOG_TAG = "BluetoothReader";

    private byte[] data = null;

    public int ReadMessage(InputStream in) throws IOException
    {
        int dataLen = 0;

        while (in.available() < NUM_BYTES_PER_INT)
        {
            try {
                Thread.sleep(100);
            }
            catch(Exception e)
            {
                Log.e("Bluetooth Reader", "Sleep Interrupted", e);
            }
        }

        byte[] bMessageLen = new byte[NUM_BYTES_PER_INT];

        int read_Amount = in.read(bMessageLen);

        if (read_Amount < NUM_BYTES_PER_INT)
        {
            throw new IOException();
        }


        while (in.available() < NUM_BYTES_PER_INT)
        {
            try {
                Thread.sleep(100);
            } catch (Exception e) {
                Log.e("Bluetooth Reader", "Sleep Interrupted", e);
            }
        }

        byte[] bMessageType = new byte[NUM_BYTES_PER_INT];
        read_Amount = in.read(bMessageType);

        if (read_Amount < bMessageType.length)
        {
            throw new IOException();
        }

        int messageType = byteArrayToInt(bMessageType);

        dataLen = byteArrayToInt(bMessageLen);
        Log.d(LOG_TAG, Integer.toString(dataLen));


        if (dataLen - bMessageType.length >= 1)
        {
            int remDataLen = dataLen - bMessageType.length;
            int readSize = 20;

            data = new byte[remDataLen];

            while(remDataLen > 0)
            {
                while (!(in.available() > readSize || in.available() > remDataLen))
                {
                    try {
                        Thread.sleep(100);
                    } catch (Exception e) {
                        Log.e("Bluetooth Reader", "Sleep Interrupted", e);
                    }
                }

                read_Amount = in.read(data, dataLen - remDataLen - bMessageType.length, in.available());
                remDataLen -= read_Amount;
            }
        }
        else
        {
            data = null;
        }

        return messageType;
    }

    public byte[] GetData(){return data;}
    public int GetDataAsInt(){return byteArrayToInt(data);}
    public int[] GetDataAsTwoInts() throws IllegalArgumentException
    {
        byte[] bArg1 = new byte[NUM_BYTES_PER_INT];
        byte[] bArg2 = new byte[NUM_BYTES_PER_INT];

        if (data.length != bArg1.length + bArg2.length)
        {
            Log.e(LOG_TAG, "Lengths " + Integer.toString(bArg1.length + bArg2.length) + "and " + Integer.toString(data.length) + " do not match.");
            throw new IllegalArgumentException();
        }

        for (int i = 0; i < bArg1.length; ++i)
        {
            bArg1[i] = data[i];
        }

        for (int i = 0; i < bArg2.length; ++i)
        {
            bArg2[i] = data[i + bArg1.length];
        }

        int[] result = new int[2];

        result[0] = byteArrayToInt(bArg1);
        result[1] = byteArrayToInt(bArg2);

        return result;
    }

    private static int byteArrayToInt(byte[] b)
    {
        final ByteBuffer bb = ByteBuffer.wrap(b);
        bb.order(ByteOrder.LITTLE_ENDIAN);
        return bb.getInt();
    }
}
