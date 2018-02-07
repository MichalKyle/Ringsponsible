package edu.illinois.echoes;

public class CustBluetooth
{
    protected static int MESSAGE_START = 0;
    protected static int MESSAGE_ACK = 1;
    protected static int MESSAGE_START_PLAYING_CHIRP = 2;
    protected static int MESSAGE_STOP = 3;
    protected static int MESSAGE_FINISH_PLAYING_CHIRP = 4;
    protected static int MESSAGE_AUDIO_DATA = 5;

    protected static int NUM_BYTES_PER_INT = Integer.SIZE / Byte.SIZE;
}

