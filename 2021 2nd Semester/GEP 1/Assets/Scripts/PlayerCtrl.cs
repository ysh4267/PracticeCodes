﻿using UnityEngine;
using System.Collections;

public class PlayerCtrl : MonoBehaviour {
	const float RayCastMaxDistance = 100.0f;
	[SerializeField] InputManager inputManager;
	[SerializeField] CharacterMove characterMove;
	// Use this for initialization
	void Start () {
		//inputManager = FindObjectOfType<InputManager>();
	}
	
	// Update is called once per frame
	void Update () {
		Walking();
	}
	
	
	
	void Walking()
	{
		if (inputManager.Clicked()) {
			Vector2 clickPos = inputManager.GetCursorPosition();
			// RRayCast로 대상물을 조사한다.
			Ray ray = Camera.main.ScreenPointToRay(clickPos);
			RaycastHit hitInfo;
			if(Physics.Raycast(ray,out hitInfo,RayCastMaxDistance,1 << LayerMask.NameToLayer("Ground"))) {
				characterMove.SetDestination(hitInfo.point);
			}
		}
	}
}
