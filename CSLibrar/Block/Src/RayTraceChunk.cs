using UnityEngine;
using System.Collections;
using System;

namespace Block
{
    public class RayTraceChunk
    {
        public RayTraceChunk() { }
        ~RayTraceChunk() { }

        byte[, ,] data;
        int sizeX, sizeY, sizeZ;//尺寸
        int byteNumZ;//z轴由几个Byte组成
        int entityNum = 0;//chunk内实心体个数

        public void create(int dx, int dy, int dz)
        {
            byteNumZ = (dz - 1) / 8 + 1;
            data = new Byte[dx, dy, byteNumZ];
            this.sizeX = dx; this.sizeY = dy; this.sizeZ = dz;
            entityNum = 0;
        }
        public int getSizeX() { return sizeX; }
        public int getSizeY() { return sizeY; }
        public int getSizeZ() { return sizeZ; }
        public void clearAll()
        {
            data = new Byte[sizeX,sizeY,byteNumZ];
            entityNum = 0;
        }
        public void clearBlock(int x, int y, int z)
        {
            byte oldData = data[x, y, z / 8];
            data[x, y, z / 8] &= (Byte)~(1 << (z % 8));
            if (oldData != data[x, y, z / 8]) {
                entityNum -= 1;
            }
        }
        public void setBlock(int x, int y, int z)
        {
            byte oldData = data[x, y, z / 8];
            data[x, y, z / 8] |= (Byte)(1 << (z % 8));
            if (oldData != data[x, y, z / 8]) {
                entityNum += 1;
            }
        }
        public bool testBlock(int x, int y, int z)
        {
            //if (x >= sizeX || y >= sizeY || z >= sizeZ) {
            //    int a = 0;
            //}
            return (data[x, y, z / 8] & (1 << (z % 8))) != 0;
        }
        //public byte[, ,] getData() { return data;}
        public bool isEmpty() { return entityNum == 0; }
        public bool isFull() { return entityNum == (data.GetLength(0) * data.GetLength(1) * data.GetLength(2)); }
    };
}
