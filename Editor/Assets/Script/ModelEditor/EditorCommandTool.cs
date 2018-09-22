using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public abstract class EditorCommandBase
{
    public abstract void Do(Block.BlockManager bm);
    public abstract void Undo(Block.BlockManager bm);
}

public class EditorCommandSimple:EditorCommandBase
{
    public delegate void FunVoid(Block.BlockManager bm);
    public FunVoid funDo;
    public FunVoid funUndo;
    public override void Do(Block.BlockManager bm) {
        funDo(bm);
    }
    public override void Undo(Block.BlockManager bm) {
        funUndo(bm);
    }
}

public class EditorCommandSet:EditorCommandBase
{
    int x, y, z, dx, dy, dz;
    short[, ,] oldTypes;
    short newType;
    public EditorCommandSet(int x, int y, int z, int dx, int dy, int dz, Block.BlockManager bm, short newType) {
        if (dx < 0) {
            x += dx + 1;
            dx = -dx;
        }
        if (dy < 0) {
            y += dy + 1;
            dy = -dy;
        }
        if (dz < 0) {
            z += dz + 1;
            dz = -dz;
        }
        this.dx = dx; this.dy = dy; this.dz = dz;      
        this.x = x; this.y = y; this.z = z;
        oldTypes = new short[dx, dy, dz];
        for (int i = 0; i < dx; i++) {
            for (int j = 0; j < dy; j++) {
                for (int k = 0; k < dz; k++) {
                    oldTypes[i, j, k] = bm.getBlock(x + i, y + j, z + k);
                }
            }
        }
        this.newType = newType;
    }
    public override void Do(Block.BlockManager bm) {
        for (int i = 0; i < dx; i++) {
            for (int j = 0; j < dy; j++) {
                for (int k = 0; k < dz; k++) {
                    bm.setBlock(x + i, y + j, z + k, newType);
                }
            }
        }        
    }
    public override void Undo(Block.BlockManager bm) {
        for (int i = 0; i < dx; i++) {
            for (int j = 0; j < dy; j++) {
                for (int k = 0; k < dz; k++) {
                    bm.setBlock(x + i, y + j, z + k, oldTypes[i, j, k]);
                }
            }
        }          
    }
}

public class EditorCommandManager
{
    public List<EditorCommandBase> commands = new List<EditorCommandBase>();
    int cmdLength;
    public void Clear() {
        commands.Clear();
        cmdLength = 0;
    }
    public void Do(EditorCommandBase cmd,Block.BlockManager bm) {
        cmd.Do(bm);
        for (int i = commands.Count - 1; i >= cmdLength; i--) {
            commands.RemoveAt(i);
        }
        commands.Add(cmd);
        cmdLength = commands.Count;
    }
    public void Undo(Block.BlockManager bm) {
        if (cmdLength > 0) {
            commands[cmdLength - 1].Undo(bm);
            cmdLength -= 1;
        }
    }

    public void Redo(Block.BlockManager bm) {
        if (commands.Count > cmdLength) {
            commands[cmdLength].Do(bm);
            cmdLength += 1;
        }
    }
}