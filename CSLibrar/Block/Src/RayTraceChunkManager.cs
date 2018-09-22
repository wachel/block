using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;

namespace Block
{
    public struct RayCastRestult
    {
        public bool bHit;
        public float hitLength;
        public int hitFaceIndex;
    };


    public class RayTraceChunkManager
    {
        public RayTraceChunkManager() { }
        ~RayTraceChunkManager() { }
        public void create(int chunkNumX, int chunkNumY, int chunkNumZ, int chunkSize)
        {
            curStartChunkX = curStartChunkY = curStartChunkZ = -65536;
            sizeX = chunkNumX; sizeY = chunkNumY; sizeZ = chunkNumZ;
            this.chunkSize = chunkSize;
            chunks = new RayTraceChunk[sizeX,sizeY,sizeZ];
            for (int k = 0; k < sizeZ; k++)
            {
                for (int j = 0; j < sizeY; j++)
                {
                    for (int i = 0; i < sizeX; i++)
                    {
                        RayTraceChunk temp = new RayTraceChunk();
                        temp.create(chunkSize, chunkSize, chunkSize);
                        temp.clearAll();
                        chunks[i,j,k] = temp;
                    }
                }
            }
        }
        public delegate bool[] GetChunkFun(int x, int y, int z);
        public void moveTo(int startChunkX, int startChunkY, int startChunkZ, GetChunkFun getChunk)
        {
            RayTraceChunk[,,] oldChunks = new RayTraceChunk[chunks.GetLength(0),chunks.GetLength(1),chunks.GetLength(2)];
            for (int k = 0; k < sizeZ; k++) {
                for (int j = 0; j < sizeY; j++) {
                    for (int i = 0; i < sizeX; i++) {
                        oldChunks[i, j, k] = chunks[i, j, k];
                    }
                }
            }

            int dx = startChunkX - curStartChunkX;
            int dy = startChunkY - curStartChunkY;
            int dz = startChunkZ - curStartChunkZ;
            for (int k = 0; k < sizeZ; k++)
            {
                for (int j = 0; j < sizeY; j++)
                {
                    for (int i = 0; i < sizeX; i++)
                    {
                        int fromX = i + dx;
                        int fromY = j + dy;
                        int fromZ = k + dz;
                        chunks[i,j,k] = oldChunks[(fromX % sizeX) , (fromY % sizeY) , (fromZ % sizeZ)];
                        if (fromX < 0 || fromX >= sizeX || fromY < 0 || fromY >= sizeY || fromZ < 0 || fromZ >= sizeZ)
                        {
                            //此格子不在原范围，需要更新
                            bool[] temp = getChunk(startChunkX + i, startChunkY + j, startChunkZ + k);
                            updateChunk(temp, chunkSize, chunks[i,j,k]);
                        }
                    }
                }
            }
            curStartChunkX = startChunkX;
            curStartChunkY = startChunkY;
            curStartChunkZ = startChunkZ;
        }
        public RayCastRestult rayCast(Vector3 startPos, Vector3 dir, float length)
        {
            RayCastRestult outResult;
            //求局部坐标，左下角为(0,0,0),单位长度为chunkSize
            Vector3 localStartPos = startPos - new Vector3(curStartChunkX, curStartChunkY, curStartChunkZ);
            RayTraceResult rlt = RayTrace.rayTrace(localStartPos, dir, this, length);//进行粗糙范围射线追踪
            outResult.bHit = rlt.bHit;
            if (rlt.hitLength > length) {
                outResult.bHit = false;
            }
            outResult.hitFaceIndex = (int)rlt.faceIndex;
            outResult.hitLength = rlt.hitLength;
            return outResult;
        }
        public RayCastRestult[] batchRayCast(Vector3[] startPos, Vector3[] dir, float[] lenght) {
            int num = startPos.Length;
            RayCastRestult[] rlts = new RayCastRestult[num];
            for (int i = 0; i < num; i++) {
                rlts[i] = rayCast(startPos[i], dir[i], lenght[i]);
            }
            return rlts;
        }
        public void update(int chunkX, int chunkY, int chunkZ, int x, int y, int z, bool bEntity) {
            if (bEntity) {
                chunks[chunkX, chunkY, chunkZ].setBlock(x, y, z);
            }
            else {
                chunks[chunkX, chunkY, chunkZ].clearBlock(x, y, z);
            }
        }
        public void updateBlock(int gx,int gy, int gz, bool bEntity) {
            if (gx >= 0 && gx < sizeX * chunkSize && gy >= 0 && gy < sizeY * chunkSize && gz >= 0 && gz < sizeZ * chunkSize) {
                int cx = gx / chunkSize;
                int cy = gy / chunkSize;
                int cz = gz / chunkSize;
                int x = gx % chunkSize;
                int y = gy % chunkSize;
                int z = gz % chunkSize;
                if (bEntity) {
                    chunks[cx, cy, cz].setBlock(x, y, z);
                }
                else {
                    chunks[cx, cy, cz].clearBlock(x, y, z);
                }
            }
        }
        private void updateChunk(bool[] data, int chunkSize, RayTraceChunk chunk)
        {
            chunk.clearAll();
            for (int k = 0; k < chunkSize; k++)
            {
                for (int j = 0; j < chunkSize; j++)
                {
                    for (int i = 0; i < chunkSize; i++)
                    {
                        if (data[k * chunkSize * chunkSize + j * chunkSize + i])
                        {
                            chunk.setBlock(i, j, k);
                        }
                    }
                }
            }
        }
        public void clearAll()
        {
            foreach (RayTraceChunk ch in chunks) {
                ch.clearAll();
            }
        }
        public bool testBlock(int x, int y, int z) {
            return chunks[x / chunkSize, y / chunkSize, z / chunkSize].testBlock(x % chunkSize, y % chunkSize, z % chunkSize);
        }

        public int getSizeX() { return sizeX * chunkSize; }
        public int getSizeY() { return sizeY * chunkSize; }
        public int getSizeZ() { return sizeZ * chunkSize; }
        public Vector3 getSize() {
            return new Vector3(sizeX, sizeY, sizeZ) * chunkSize;
        }
        public VecInt3 getChunkSize()
        {
            return new VecInt3(sizeX, sizeY, sizeZ);
        }
        public RayTraceChunk getChunk(int cx, int cy, int cz)
        {
            return chunks[cx, cy, cz];
        }
        public bool isChunkEmpty(int cx,int cy,int cz)
        {
            return chunks[cx,cy,cz].isEmpty();
        }
        public int ChunkSize { get { return chunkSize; } }
        int sizeX, sizeY, sizeZ;	//每个方向有多少个chunk
        int chunkSize;		//每个chunk的每个方向有多少个block
        int curStartChunkX, curStartChunkY, curStartChunkZ;//起始区域是哪个chunk
        RayTraceChunk[,,] chunks;//每个具体块
    };
}
