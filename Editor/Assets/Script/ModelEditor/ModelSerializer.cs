using UnityEngine;
using System.Collections;

public class ModelSerializer
{
    public static void SaveModel(Block.BlockManager bm,VecInt3 pivot, string path) {
        //Block.BoundsInt bounds = new Block.BoundsInt();
        //bm.forEachChunk((Block.BlockChunk chunk, int cx, int cy, int cz) => {
        //    if (!chunk.isEmpty()) {
        //        for (int z = 0; z < Block.Const.ChunkSize; z++) {
        //            for (int x = 0; x < Block.Const.ChunkSize; x++) {
        //                for (int y = 0; y < Block.Const.ChunkSize; y++) {
        //                    int gx = cx * Block.Const.ChunkSize + x;
        //                    int gy = cy * Block.Const.ChunkSize + y;
        //                    int gz = cz * Block.Const.ChunkSize + z;
        //                    short blockType = bm.getBlock(gx, gy, gz);
        //                    if (bm.blockTypeFun.isOpacity(blockType)) {
        //                        bounds.Encapsulate(new Block.VecInt3(gx, gy, gz));
        //                    }
        //                }
        //            }
        //        }
        //    }
        //});
        //if (bounds.IsValid()) {
        //    BlockData.Model model = new BlockData.Model();
        //    model.version = 0;
        //    model.sizeX = bounds.Size.x;
        //    model.sizeY = bounds.Size.y;
        //    model.sizeZ = bounds.Size.z;
        //    model.pivotX = pivot.x - bounds.min.x;
        //    model.pivotY = pivot.y - bounds.min.y;
        //    model.pivotZ = pivot.z - bounds.min.z;
        //    
        //    int num = 0;
        //    short lastType = bm.getBlock(bounds.min.x, bounds.min.y, bounds.min.z);
        //    for (int z = bounds.min.z; z <= bounds.max.z; z++) {
        //        for (int x = bounds.min.z; x <= bounds.max.x; x++) {
        //            for (int y = bounds.min.y; y <= bounds.max.y; y++) {
        //                short curType = bm.getBlock(x, y, z);
        //                if (lastType == curType) {
        //                    num++;
        //                }
        //                else {
        //                    BlockData.BlockPair block = new BlockData.BlockPair();
        //                    block.lenght = num;
        //                    block.type = lastType;
        //                    model.blocks.Add(block);
        //                    lastType = curType;
        //                    num = 1;
        //                }
        //            }
        //        }
        //    }
        //    BlockData.BlockPair blockLast = new BlockData.BlockPair();
        //    blockLast.lenght = num;
        //    blockLast.type = lastType;
        //    model.blocks.Add(blockLast);
        //    Write<BlockData.Model>(model, path);
        //}

    }

    public static BlockData.Model LoadModel(string path) {
        BlockData.Model model = Read<BlockData.Model>(path);
        return model;
    }

    private static void Write<T>(T mdl, string path) {
        if (!System.IO.Directory.Exists(path)) {
            if (System.IO.File.Exists(path)) {
                System.IO.File.Delete(path);
            }
            System.IO.FileStream stream = System.IO.File.OpenWrite(path);
            ProtoBuf.Serializer.Serialize<T>(stream, mdl);
            stream.Close();
        }
    }
    private static T Read<T>(string path) {
        System.IO.FileStream stream = System.IO.File.OpenRead(path);
        T rlt = ProtoBuf.Serializer.Deserialize<T>(stream);
        return rlt;
    }
}
