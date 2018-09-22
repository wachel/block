using UnityEngine;
using System.Collections;
using System;


namespace Block
{
    public class BlockManager
    {
        private int sizeX;
        private int sizeY;
        private int sizeZ;

        public int baseX;
        public int baseY;
        public int baseZ;

        private short[, ,] blocks;
        public BlockTypeFunBase blockTypeFun;

        public BlockManager() { }
        ~BlockManager() { }
        public void create(int sizeX, int sizeY, int sizeZ,BlockTypeFunBase blockTypeFun)
        {
            this.sizeX = sizeX;
            this.sizeY = sizeY;
            this.sizeZ = sizeZ;
            this.blockTypeFun = blockTypeFun;
            blocks = new short[sizeX, sizeY, sizeZ];

            //Array.Resize(ref chunks, chunkNumX * chunkNumY * chunkNumZ);
            //for (int i = 0; i < chunks.Length; i++)
            //{
            //    chunks[i] = new BlockChunk(blockTypeFun);
            //}
            //forEachChunk((chunk, i, j, k) =>
            //{
            //    for (int x = -1; x <= 1; x++)
            //    {
            //        for (int y = -1; y <= 1; y++)
            //        {
            //            for (int z = -1; z <= 1; z++)
            //            {
            //                // int index = (z+1)*9+(y+1)*3+x+1;
            //                if (i + x >= 0 && i + x < chunkNumX && j + y >= 0 && j + y < chunkNumY && k + z >= 0 && k + z < chunkNumZ)
            //                {
            //                    chunk.setAdjacencyChunk(x, y, z, getChunk(i + x, j + y, k + z));
            //                }
            //            }
            //        }
            //    }
            //});
        }
        //public BlockChunk getChunk(int cx, int cy, int cz)
        //{
        //    int index = cz * chunkNumY * chunkNumX + cy * chunkNumX + cx;
        //    return chunks[index];
        //}
        public void setBlock(int gx, int gy, int gz,short blockType) {
            //if (gx >= 0 && gx < chunkNumX * Const.ChunkSize && gy >= 0 && gy < chunkNumY * Const.ChunkSize && gz >= 0 && gz < chunkNumZ * Const.ChunkSize) {
            //    int cx = gx / Const.ChunkSize;
            //    int cy = gy / Const.ChunkSize;
            //    int cz = gz / Const.ChunkSize;
            //    int x = gx % Const.ChunkSize;
            //    int y = gy % Const.ChunkSize;
            //    int z = gz % Const.ChunkSize;
            //    getChunk(cx,cy,cz).setBlock(x,y,z, blockType);
            //}
            if (gx >= 0 && gx < sizeX && gy >= 0 && gy < sizeY && gz >= 0 && gz < sizeZ) {
                blocks[gx % sizeX, gy % sizeY, gz % sizeZ] = blockType;
            }
        }
        public void clear()
        {
            for (int i = 0; i < sizeX; i++) {
                for (int j = 0; j < sizeX; j++) {
                    for (int k = 0; i < sizeX; k++) {
                        blocks[i, j, k] = 0;
                    }
                }
            }
        }
        public short getBlock(int gx, int gy, int gz) {
            //if (gx >= 0 && gx < chunkNumX * Const.ChunkSize && gy >= 0 && gy < chunkNumY * Const.ChunkSize && gz >= 0 && gz < chunkNumZ * Const.ChunkSize) {
            //    int cx = gx / Const.ChunkSize;
            //    int cy = gy / Const.ChunkSize;
            //    int cz = gz / Const.ChunkSize;
            //    int x = gx % Const.ChunkSize;
            //    int y = gy % Const.ChunkSize;
            //    int z = gz % Const.ChunkSize;
            //    return getChunk(cx, cy, cz).getBlock(x, y, z);
            //}
            //return (short)BlockTypeEnum.Air;
           
            if (gx >= 0 && gx < sizeX && gy >= 0 && gy < sizeY && gz >= 0 && gz < sizeZ) {
                return blocks[gx, gy, gz];
            }
            return 0;
        }
        //public delegate void ChunkFun(BlockChunk chunk, int chunkX, int chunkY, int chunkZ);
        //public void forEachChunk(ChunkFun fun)
        //{
        //    for (int i = 0; i < chunkNumX; i++)
        //    {
        //        for (int j = 0; j < chunkNumY; j++)
        //        {
        //            for (int k = 0; k < chunkNumZ; k++)
        //            {
        //                fun(getChunk(i,j,k),i,j,k);
        //            }
        //        }
        //    }
        //}
        public delegate void BlockFun(short block, int x, int y, int z);
        public void forEachBlock(BlockFun fun)
        {
            for (int i = 0; i < sizeX; i++) {
                for (int j = 0; j < sizeX; j++) {
                    for (int k = 0; i < sizeX; k++) {
                        fun(getBlock(i + baseX, j + baseY, k + baseZ), i + baseX, j + baseY, k + baseZ);
                    }
                }
            }
            //    forEachChunk((BlockChunk chunk, int cx, int cy, int cz) => {
            //        chunk.forEachBlock((int x, int y, int z) => {
            //            fun(chunk.getBlock(x, y, z), cx * Const.ChunkSize + x, cy * Const.ChunkSize + y, cz * Const.ChunkSize + z);
            //        });
            //    });
        }
        public int SizeX { get { return sizeX; } }
        public int SizeY { get { return sizeY; } }
        public int SizeZ { get { return sizeZ; } }
        //public int BlockSizeX { get { return chunkNumX * Const.ChunkSize; } }
        //public int BlockSizeY { get { return chunkNumY * Const.ChunkSize; } }
        //public int BlockSizeZ { get { return chunkNumZ * Const.ChunkSize; } }
        //
        //public int getSizeX() { return chunkNumX; }
        //public int getSizeY() { return chunkNumY; }
        //public int getSizeZ() { return chunkNumZ; }
        //private BlockChunk[] chunks;
        //private int chunkNumX;
        //private int chunkNumY;
        //private int chunkNumZ;
        //public BlockTypeFunBase blockTypeFun;
    };
}