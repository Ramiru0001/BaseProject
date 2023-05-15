using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Tank : MonoBehaviour
{
    public Vector3 m_rot=new Vector3(0,0,0);
    public GameObject m_top;
    public GameObject m_gun;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        float rot_speed = 0.2f;
        m_rot += new Vector3(Input.GetAxis("Mouse Y"), Input.GetAxis("Mouse X"), 0) * rot_speed;
        m_rot.x = Mathf.Clamp(m_rot.x, -45, 0);
        m_top.transform.localEulerAngles = new Vector3(0, m_rot.y, 0);
        m_gun.transform.localEulerAngles = new Vector3(m_rot.x, 0, 0);


    }
}
