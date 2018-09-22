using UnityEngine;
using System.Collections;

public class CameraController 
{
    Camera camModel;
    //Vector2 mouseDownPosition;
    Vector2 lastMousePosition;
    Vector3 eulerAngle;
    enum State
    {
        Nothing,
        Look,
        Move,
    }
    //State curState;
    Vector2 oldPos;
    public void Init(Camera model) {
        camModel = model;
        camModel.gameObject.transform.position = Vector3.back * 30 + Vector3.up * 20;
        camModel.gameObject.transform.rotation = Quaternion.AngleAxis(-30,Vector3.left);
        eulerAngle = camModel.gameObject.transform.eulerAngles;
        //curState = State.Nothing;
    }
    public void OnGUI() {
        if (Input.touchCount == 2) {
            Vector2 move = (Input.touches[0].deltaPosition + Input.touches[1].deltaPosition) / 2;
            camModel.gameObject.transform.position -= camModel.gameObject.transform.TransformDirection(Vector3.right) * move.x * 0.03f;
            camModel.gameObject.transform.position -= camModel.gameObject.transform.TransformDirection(Vector3.up) * move.y * 0.03f;

            Vector2 forward = (Input.touches[1].deltaPosition - Input.touches[0].deltaPosition);
            forward = Input.touches[1].position.x > Input.touches[0].position.x ? forward : -forward;
            camModel.gameObject.transform.position += camModel.gameObject.transform.TransformDirection(Vector3.forward) * forward.x * 0.1f;
            eulerAngle.x += forward.y * 0.1f;
            camModel.gameObject.transform.rotation = Quaternion.Euler(eulerAngle);
            return;
        }
        else if (Input.touchCount == 3) {
            Vector2 move = (Input.touches[0].deltaPosition + Input.touches[1].deltaPosition + Input.touches[2].deltaPosition) / 3;

            float speed = 0.1f;
            eulerAngle.y -= move.x * speed;
            eulerAngle.x += move.y * speed;
            camModel.gameObject.transform.rotation = Quaternion.Euler(eulerAngle);            
        }

        Event e = Event.current;
        if (e != null) {
            if (e.type == EventType.MouseDown) {
                //mouseDownPosition = e.mousePosition;
                lastMousePosition = e.mousePosition;
            }
            else if (e.type == EventType.MouseDrag) {
                Vector2 offset = e.mousePosition - lastMousePosition;
                if (e.button == 2) {
                    float speed = 0.004f * camModel.transform.position.magnitude;
                    camModel.gameObject.transform.position -= camModel.gameObject.transform.TransformDirection(Vector3.right) * offset.x * speed;
                    camModel.gameObject.transform.position += camModel.gameObject.transform.TransformDirection(Vector3.up) * offset.y * speed;
                }
                else if (e.button == 1) {
                    float speed = 0.2f;
                    eulerAngle.y += offset.x * speed;
                    eulerAngle.x += offset.y * speed;
                    camModel.gameObject.transform.rotation = Quaternion.Euler(eulerAngle);
                }
                lastMousePosition = e.mousePosition;
            }
            else if (e.type == EventType.ScrollWheel) {
                float speed = 0.1f * camModel.transform.position.magnitude;
                camModel.gameObject.transform.position -= camModel.gameObject.transform.TransformDirection(Vector3.forward) * e.delta.y * speed;
            }
        }
    }
}
