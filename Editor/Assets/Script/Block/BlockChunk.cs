using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace Block
{
    public class BlockChunk
    {
        BlockManager blockManager;
        int baseX, baseY, baseZ;
        public BlockChunk(BlockManager bm, int baseX, int baseY, int baseZ)
        {
            this.blockManager = bm;
            this.baseX = baseX; this.baseY = baseY; this.baseZ = baseZ;
        }
        public short getBlock(int x, int y, int z)
        {
            return blockManager.getBlock(x + baseX, y + baseY, z + baseZ);
        }
    }
    //public class BlockChunk
    //{
    //    public BlockChunk(BlockTypeFunBase fun)
    //    {
    //        for (int i = 0; i < 27; i++)
    //        {
    //            adjacencyChunks[i] = defaultChunk;
    //        }
    //        adjacencyChunks[13] = this;
    //        for (int i = 0; i < 6; i++) {
    //            surfaceList[i] = new List<VecInt3>();
    //        }
    //        this.blockTypeFun = fun;
    //    }
    //    
    //    ~BlockChunk() { }
    //    static BlockChunk createDefaultChunk()
    //    {
    //        BlockChunk rlt = new BlockChunk(null);
    //        for (int i = 0; i < Const.ChunkSize; i++)
    //        {
    //            for (int j = 0; j < Const.ChunkSize; j++)
    //            {
    //                for (int k = 0; k < Const.ChunkSize; k++)
    //                {
    //                    rlt.setBlock(i, j, k, (short)BlockTypeEnum.Out);
    //                }
    //            }
    //        }
    //        return rlt;
    //    }
    //
    //    public void setBlock(int x, int y, int z, short val)
    //    {
    //        if (blockTypeFun != null) {
    //            bool oldIsEntity = blockTypeFun.isOpacity(blocks[x, y, z]);
    //            bool newIsEntity = blockTypeFun.isOpacity(val);
    //            if (oldIsEntity != newIsEntity) {
    //                isSurfaceDirty = true;
    //                entityNum += oldIsEntity ? -1 : 1;
    //            }
    //        }
    //        blocks[x,y,z] = val;
    //    }
    //
    //    public short getBlockSmart(int x, int y, int z)
    //    {
    //        int xx = x < 0 ? -1 : (x >= Const.ChunkSize ? 1 : 0);
    //        int yy = y < 0 ? -1 : (y >= Const.ChunkSize ? 1 : 0);
    //        int zz = z < 0 ? -1 : (z >= Const.ChunkSize ? 1 : 0);
    //        return getAdjacencyChunk(xx, yy, zz).getBlock(x - xx * Const.ChunkSize, y - yy * Const.ChunkSize, z - zz * Const.ChunkSize);
    //    }
    //
    //    public short getBlock(int x, int y, int z)
    //    {
    //        return blocks[x,y,z];
    //    }
    //
    //    public void clear(short block)
    //    {
    //        for (int i = 0; i < Const.ChunkSize; i++) {
    //            for (int j = 0; j < Const.ChunkSize; j++) {
    //                for (int k = 0; k < Const.ChunkSize; k++) {
    //                    blocks[i,j,k] = block;
    //                }
    //            }
    //        }
    //        isSurfaceDirty = true;
    //        entityNum = 0;
    //    }
    //
    //    public bool isEmpty()
    //    {
    //        return entityNum == 0;
    //    }
    //
    //    public void setBrightness(int x, int y, int z, int faceIndex, byte val)
    //    {
    //        brightness[x, y, z, faceIndex] = val;
    //    }
    //
    //    public byte getBrightnessSmart(int x, int y, int z, int faceIndex)
    //    {
    //        int xx = x < 0 ? -1 : (x >= Const.ChunkSize ? 1 : 0);
    //        int yy = y < 0 ? -1 : (y >= Const.ChunkSize ? 1 : 0);
    //        int zz = z < 0 ? -1 : (z >= Const.ChunkSize ? 1 : 0);
    //        return getAdjacencyChunk(xx, yy, zz).getBrightness(x - xx * Const.ChunkSize, y - yy * Const.ChunkSize, z - zz * Const.ChunkSize, faceIndex);
    //    }
    //
    //    public byte getBrightness(int x, int y, int z, int faceIndex)
    //    {
    //        return brightness[x,y,z,faceIndex];
    //    }
    //
    //    public void setAdjacencyChunk(int x, int y, int z, BlockChunk chunk)
    //    {
    //        adjacencyChunks[(z + 1) * 9 + (y + 1) * 3 + (x + 1)] = chunk;
    //    }
    //
    //    public BlockChunk getAdjacencyChunk(int x, int y, int z)
    //    {
    //        return adjacencyChunks[(z + 1) * 9 + (y + 1) * 3 + (x + 1)];
    //    }
    //
    //    public delegate void BlockFun(int x, int y, int z);
    //    public void forEachBlock(BlockFun fun){
    //        for(int i = 0; i<Const.ChunkSize; i++){
    //            for(int j = 0;j<Const.ChunkSize; j++){
    //                for(int k =0; k<Const.ChunkSize;k++){
    //                    fun(i, j, k);
    //                }
    //            }
    //        }
    //    }
    //
    //    public bool[] getEntityArray(BlockTypeFunBase blockType) {
    //        bool[] data = new bool[Const.ChunkSize * Const.ChunkSize * Const.ChunkSize];
    //        for(int i = 0; i< blocks.Length; i++){
    //            int x = i % Const.ChunkSize;
    //            int y = (i / Const.ChunkSize) % Const.ChunkSize;
    //            int z = i / (Const.ChunkSize * Const.ChunkSize);
    //            data[i] = blockType.isOpacity(blocks[x,y,z]);
    //        }
    //        return data;
    //    }
    //
    //    public List<VecInt3>[] calcSurfaceList() {
    //        BlockTypeFunBase fun = blockTypeFun;
    //        if (isSurfaceDirty) {
    //            isSurfaceDirty = false;
    //            surfaceList = new List<VecInt3>[6];
    //            for (int f = 0; f < 6; f++) {
    //                surfaceList[f] = new List<VecInt3>();
    //            }
    //            for (int z = 0; z < Const.ChunkSize; z++) {
    //                for (int y = 0; y < Const.ChunkSize; y++) {
    //                    bool bOldIsEntity = fun.isOpacity(getBlockSmart(-1, y, z));
    //                    for (int x = 0; x < Const.ChunkSize; x++) {
    //                        bool bCurIsEntity = fun.isOpacity(getBlock(x, y, z));
    //                        if (bCurIsEntity && !bOldIsEntity) {
    //                            surfaceList[(int)Face.FNI_x0].Add(new VecInt3(x, y, z));
    //                        }
    //                        else if (bOldIsEntity && (!bCurIsEntity) && x >= 1) {
    //                            surfaceList[(int)Face.FNI_x1].Add(new VecInt3(x - 1, y, z));
    //                        }
    //                        bOldIsEntity = bCurIsEntity;
    //                    }
    //                    if (bOldIsEntity && !fun.isOpacity(getBlockSmart(Const.ChunkSize, y, z))) {
    //                        surfaceList[(int)Face.FNI_x1].Add(new VecInt3(Const.ChunkSize - 1, y, z));
    //                    }
    //                }
    //            }
    //            for (int z = 0; z < Const.ChunkSize; z++) {
    //                for (int x = 0; x < Const.ChunkSize; x++) {
    //                    bool bOldIsEntity = fun.isOpacity(getBlockSmart(x, -1, z));
    //                    for (int y = 0; y < Const.ChunkSize; y++) {
    //                        bool bCurIsEntity = fun.isOpacity(getBlock(x, y, z));
    //                        if (bCurIsEntity && !bOldIsEntity) {
    //                            surfaceList[(int)Face.FNI_y0].Add(new VecInt3(x, y, z));
    //                        }
    //                        else if (bOldIsEntity && (!bCurIsEntity) && y >= 1) {
    //                            surfaceList[(int)Face.FNI_y1].Add(new VecInt3(x, y - 1, z));
    //                        }
    //                        bOldIsEntity = bCurIsEntity;
    //                    }
    //                    if (bOldIsEntity && !fun.isOpacity(getBlockSmart(x, Const.ChunkSize, z))) {
    //                        surfaceList[(int)Face.FNI_y1].Add(new VecInt3(x,Const.ChunkSize - 1, z));
    //                    }
    //                }
    //            }
    //            for (int x = 0; x < Const.ChunkSize; x++) {
    //                for (int y = 0; y < Const.ChunkSize; y++) {
    //                    bool bOldIsEntity = fun.isOpacity(getBlockSmart(x, y, -1));
    //                    for (int z = 0; z < Const.ChunkSize; z++) {
    //                        bool bCurIsEntity = fun.isOpacity(getBlock(x, y, z));
    //                        if (bCurIsEntity && !bOldIsEntity) {
    //                            surfaceList[(int)Face.FNI_z0].Add(new VecInt3(x, y, z));
    //                        }
    //                        else if (bOldIsEntity && (!bCurIsEntity) && z >= 1) {
    //                            surfaceList[(int)Face.FNI_z1].Add(new VecInt3(x, y, z - 1));
    //                        }
    //                        bOldIsEntity = bCurIsEntity;
    //                    }
    //                    if (bOldIsEntity && !fun.isOpacity(getBlockSmart(x,y, Const.ChunkSize))) {
    //                        surfaceList[(int)Face.FNI_z1].Add(new VecInt3(x,y, Const.ChunkSize - 1));
    //                    }
    //                }
    //            }
    //        }
    //
    //        return surfaceList;
    //    }
    //
    //    private BlockChunk[] adjacencyChunks = new BlockChunk[27];
    //    private short[,,] blocks = new short[Const.ChunkSize , Const.ChunkSize , Const.ChunkSize];
    //    private byte[,,,] brightness = new byte[Const.ChunkSize , Const.ChunkSize , Const.ChunkSize , 6];//面的光照强度,
    //    private static readonly BlockChunk defaultChunk = createDefaultChunk();
    //    private List<VecInt3>[] surfaceList = new List<VecInt3>[6];
    //    private bool isSurfaceDirty = true;
    //    private int entityNum;
    //    private BlockTypeFunBase blockTypeFun;
    //};
    //
}