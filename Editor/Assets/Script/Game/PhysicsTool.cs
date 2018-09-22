using UnityEngine;
using System.Collections;

public static class PhysicsTool
{
    public static VecInt3 getBlockPos(Vector3 globalPos)
    {
        return new VecInt3((int)globalPos.x, (int)globalPos.y, (int)globalPos.z);
    }
    public static Vector3 getLocalPosInBlock(Vector3 globalPos)
    {
        return new Vector3(globalPos.x - (int)globalPos.x, globalPos.y - (int)globalPos.y, globalPos.z - (int)globalPos.z);
    }

    public static bool isOnBlock(Block.BlockManager bm, Vector3 pos)
    {
        VecInt3 bpos = getBlockPos(pos - Vector3.up * 0.1f);
        short block = bm.getBlock(bpos.x, bpos.y, bpos.z);
        if (bm.blockTypeFun.isCollider(block)) {
            return true;
        }
        return false;
    }
}