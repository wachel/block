using UnityEngine;
using System.Collections;
using Block;
using System;

public class MoveController:MonoBehaviour
{
    public Avatar avatar;
    public Level level;
    int height = 3;
    void OnEnable()
    {
        Cursor.visible = false;
        Cursor.lockState = CursorLockMode.Locked;
    }

    public void OnDisable()
    {
        Cursor.visible = true;
        Cursor.lockState = CursorLockMode.None;
    }

    void Update()
    {
        if (avatar && level && level.scene != null) {
            float moveAngle = 0;
            MoveDirection dir = getKeyboardDirection();
            if (dir == MoveDirection.Stand) {
                avatar.moveSpeed = 0;
            }
            else {
                avatar.moveSpeed = avatar.config.walkSpeed;
                moveAngle = ((int)dir) * 45;
            }

            if (KeyMapping.IsKeyDown(GameKeyCode.Jump)) {
                float distanceToGround = GetDistanceToGround(level.scene, avatar.pos, height);
                if (distanceToGround < 0.15f) {
                    avatar.currentVelocity.y = 8;
                }
            }

            //平移
            Vector3 newPos = avatar.pos;
            Vector3 moveDir = Quaternion.AngleAxis(moveAngle + avatar.yaw, Vector3.up) * Vector3.forward;
            float distance = avatar.moveSpeed * Time.deltaTime;
            float maxStep = 0.2f;
            while (distance > 0) {
                float moveDistance = Mathf.Min(maxStep, distance);
                newPos = GetNearestWalkablePos(level.scene, newPos + moveDir * moveDistance, height);
                distance -= maxStep;
            }


            //垂直
            {
                float distanceToGround = GetDistanceToGround(level.scene, newPos, height);
                if (distanceToGround > 0.1f) {//不站在地面，则持续下落中
                    avatar.currentVelocity.y -= Time.deltaTime * 20;
                }

                if (avatar.currentVelocity.y < 0.01f) {//非上升过程则检测地面
                    if (distanceToGround < 0) {
                        newPos.y -= distanceToGround;
                        avatar.currentVelocity.y = 0;
                    }
                }
                else {
                    if (!IsWalkable(level.scene, newPos, height)) {
                        avatar.currentVelocity.y = 0;
                    }
                }
                newPos += Time.deltaTime * avatar.currentVelocity;
            }


            avatar.pos = newPos;
            avatar.yaw += Input.GetAxis("Mouse X") * 10;
            avatar.pitch -= Input.GetAxis("Mouse Y") * 10;
            avatar.pitch = Mathf.Clamp(avatar.pitch, -85, 85);
            UpdateCameraToAvatar();
        }
    }


    static Vector3 GetNearestWalkablePos(RayCastManager scene, Vector3 pos,int height)
    {
        float r = 0.3f;
        VecInt3 g = pos;
        Vector3 localPos = pos - g;

        VecInt3 step;//移动的方向，-1，0，1
        step.x = localPos.x < r ? -1 : (localPos.x > (1 - r) ? 1 : 0);
        step.z = localPos.z < r ? -1 : (localPos.z > (1 - r) ? 1 : 0);

        Vector3 offset;//退回到正确的位置需要移动的量
        offset.x = step.x == -1 ? r - localPos.x : (step.x == 1 ? ((1 - r) - localPos.x) : 0);
        offset.z = step.z == -1 ? r - localPos.z : (step.z == 1 ? ((1 - r) - localPos.z) : 0);

        bool bCanMoveX = step.x != 0 ? scene.isWalkable(g.x + step.x, g.y, g.z, height) : true;
        bool bCanMoveZ = step.z != 0 ? scene.isWalkable(g.x, g.y, g.z + step.z, height) : true;
        bool bCanMoveXZ = (step.x != 0 && step.z != 0) ? scene.isWalkable(g.x + step.x, g.y, g.z + step.z, height) : true;

        bool bCanUpX = scene.isWalkable(g.x + step.x, g.y + 1, g.z, height);
        bool bCanUpZ = scene.isWalkable(g.x, g.y + 1, g.z + step.z, height);
        bool bCanUpXZ = scene.isWalkable(g.x + step.x, g.y + 1, g.z + step.z, height);

        bCanMoveX |= bCanUpX;
        bCanMoveZ |= bCanUpZ;
        bCanMoveXZ |= bCanUpXZ;

        if (!bCanMoveXZ && bCanMoveX && bCanMoveZ) {
            if (Mathf.Abs(offset.x) < Mathf.Abs(offset.z)) {
                bCanMoveX = false;
            }
            else {
                bCanMoveZ = false;
            }
        }

        if (!bCanMoveX) {
            pos.x += offset.x;
        }

        if (!bCanMoveZ) {
            pos.z += offset.z;
        }

        //if ((!bCanMoveX) || (!bCanMoveZ)) {
        //    bool bCanStepUp = scene.testBlock(g.x + step.x, g.y + 1, g.z + step.z, RayCastBlockType.Walkable);
        //    if (bCanStepUp) {
        //        pos.y = g.y + 1;
        //    }
        //    else {
        //
        //    }
        //}
                  
        return pos;
    }

    static float GetDistanceToGround(RayCastManager scene,Vector3 pos,int height)
    {
        VecInt3 g = pos;
        Vector3 localPos = pos - g;
        if (scene.isWalkable(g.x, g.y - 1, g.z, height)) {
            return 1 + localPos.y;
        }
        else if (scene.isWalkable(g.x, g.y, g.z, height)) {
            return localPos.y;
        }
        else if (!scene.isWalkable(g.x, g.y, g.z, height)) {
            return localPos.y - 1;
        }
        return 1;
    }

    static bool IsWalkable(RayCastManager scene, Vector3 pos, int height)
    {
        VecInt3 g = pos;
        return scene.isWalkable(g.x, g.y, g.z, height);
    }

    void UpdateCameraToAvatar()
    {
        Quaternion rotPitch = Quaternion.AngleAxis(avatar.pitch, Vector3.right);
        Quaternion rotYaw = Quaternion.AngleAxis(avatar.yaw, Vector3.up);
        Camera.main.transform.rotation = rotYaw * rotPitch;
        Camera.main.transform.position = avatar.pos + Vector3.up * avatar.config.height * 0.9f;
    }

    private enum MoveDirection
    {
        Stand = -1,
        Up = 0,
        UpRight,
        Right,
        DownRight,
        Down,
        DownLeft,
        Left,
        UpLeft,
    };
    private MoveDirection getKeyboardDirection()
    {
        MoveDirection moveDir = MoveDirection.Stand;
        if (KeyMapping.IsKeyDown(GameKeyCode.Forward) && KeyMapping.IsKeyDown(GameKeyCode.Left)) {
            moveDir = MoveDirection.UpLeft;
        }
        else if (KeyMapping.IsKeyDown(GameKeyCode.Forward) && KeyMapping.IsKeyDown(GameKeyCode.Right)) {
            moveDir = MoveDirection.UpRight;
        }
        else if (KeyMapping.IsKeyDown(GameKeyCode.Backward) && KeyMapping.IsKeyDown(GameKeyCode.Left)) {
            moveDir = MoveDirection.DownLeft;
        }
        else if (KeyMapping.IsKeyDown(GameKeyCode.Backward) && KeyMapping.IsKeyDown(GameKeyCode.Right)) {
            moveDir = MoveDirection.DownRight;
        }
        else if (KeyMapping.IsKeyDown(GameKeyCode.Forward)) {
            moveDir = MoveDirection.Up;
        }
        else if (KeyMapping.IsKeyDown(GameKeyCode.Left)) {
            moveDir = MoveDirection.Left;
        }
        else if (KeyMapping.IsKeyDown(GameKeyCode.Right)) {
            moveDir = MoveDirection.Right;
        }
        else if (KeyMapping.IsKeyDown(GameKeyCode.Backward)) {
            moveDir = MoveDirection.Down;
        }
        return moveDir;
    }
}
