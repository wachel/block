using UnityEngine;
using System.Collections;
using System;


namespace Block
{
    public class BlockManager
    {
        public BlockManager() { }
        ~BlockManager() { }
        public void create(int chunkNumX, int chunkNumY, int chunkNumZ,BlockTypeFunBase blockTypeFun)
        {
            this.chunkNumX = chunkNumX;
            this.chunkNumY = chunkNumY;
            this.chunkNumZ = chunkNumZ;
            this.blockTypeFun = blockTypeFun;
            Array.Resize(ref chunks, chunkNumX * chunkNumY * chunkNumZ);
            for (int i = 0; i < chunks.Length; i++)
            {
                chunks[i] = new BlockChunk(blockTypeFun);
            }
            forEachChunk((chunk, i, j, k) =>
            {
                for (int x = -1; x <= 1; x++)
                {
                    for (int y = -1; y <= 1; y++)
                    {
                        for (int z = -1; z <= 1; z++)
                        {
                            // int index = (z+1)*9+(y+1)*3+x+1;
                            if (i + x >= 0 && i + x < chunkNumX && j + y >= 0 && j + y < chunkNumY && k + z >= 0 && k + z < chunkNumZ)
                            {
                                chunk.setAdjacencyChunk(x, y, z, getChunk(i + x, j + y, k + z));
                            }
                        }
                    }
                }
            });
        }
        public BlockChunk getChunk(int cx, int cy, int cz)
        {
            int index = cz * chunkNumY * chunkNumX + cy * chunkNumX + cx;
            return chunks[index];
        }
        public void setBlock(int gx, int gy, int gz,short blockType) {
            if (gx >= 0 && gx < chunkNumX * Const.ChunkSize && gy >= 0 && gy < chunkNumY * Const.ChunkSize && gz >= 0 && gz < chunkNumZ * Const.ChunkSize) {
                int cx = gx / Const.ChunkSize;
                int cy = gy / Const.ChunkSize;
                int cz = gz / Const.ChunkSize;
                int x = gx % Const.ChunkSize;
                int y = gy % Const.ChunkSize;
                int z = gz % Const.ChunkSize;
                getChunk(cx,cy,cz).setBlock(x,y,z, blockType);
            }
        }
        public short getBlock(int gx, int gy, int gz) {
            if (gx >= 0 && gx < chunkNumX * Const.ChunkSize && gy >= 0 && gy < chunkNumY * Const.ChunkSize && gz >= 0 && gz < chunkNumZ * Const.ChunkSize) {
                int cx = gx / Const.ChunkSize;
                int cy = gy / Const.ChunkSize;
                int cz = gz / Const.ChunkSize;
                int x = gx % Const.ChunkSize;
                int y = gy % Const.ChunkSize;
                int z = gz % Const.ChunkSize;
                return getChunk(cx, cy, cz).getBlock(x, y, z);
            }
            return (short)BlockTypeEnum.Air;
        }
        public delegate void ChunkFun(BlockChunk chunk, int chunkX, int chunkY, int chunkZ);
        public void forEachChunk(ChunkFun fun)
        {
            for (int i = 0; i < chunkNumX; i++)
            {
                for (int j = 0; j < chunkNumY; j++)
                {
                    for (int k = 0; k < chunkNumZ; k++)
                    {
                        fun(getChunk(i,j,k),i,j,k);
                    }
                }
            }
        }
        public delegate void BlockFun(short block, int x, int y, int z);
        public void forEachBlock(BlockFun fun)
        {
            forEachChunk((BlockChunk chunk,int cx, int cy,int cz)=>{
                chunk.forEachBlock((int x,int y ,int z)=>{
                    fun(chunk.getBlock(x, y, z), cx * Const.ChunkSize + x, cy * Const.ChunkSize + y, cz * Const.ChunkSize + z);
                });
            });
        }
        public int SizeX { get { return chunkNumX; } }
        public int SizeY { get { return chunkNumY; } }
        public int SizeZ { get { return chunkNumZ; } }
        public int BlockSizeX { get { return chunkNumX * Const.ChunkSize; } }
        public int BlockSizeY { get { return chunkNumY * Const.ChunkSize; } }
        public int BlockSizeZ { get { return chunkNumZ * Const.ChunkSize; } }

        public int getSizeX() { return chunkNumX; }
        public int getSizeY() { return chunkNumY; }
        public int getSizeZ() { return chunkNumZ; }
        private BlockChunk[] chunks;
        private int chunkNumX;
        private int chunkNumY;
        private int chunkNumZ;
        public BlockTypeFunBase blockTypeFun;
    };
}