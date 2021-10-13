﻿using UnityEngine;
using System.Collections;

public class PlayerCtrl : MonoBehaviour
{
    const float RayCastMaxDistance = 100.0f;
    CharacterStatus status;
    CharaAnimation charaAnimation;
    Transform attackTarget;
    InputManager inputManager;
    public float attackRange = 1.5f;
    GameRuleCtrl gameRuleCtrl;
    public GameObject hitEffect;
    TargetCursor targetCursor;

    // 스테이트 종류.
    enum State
    {
        Walking,
        Attacking,
        Died,
    } ;

    State state = State.Walking;		// 현재 스테이트.
    State nextState = State.Walking;	// 다음 스테이트.


    // Use this for initialization
    void Start()
    {
        status = GetComponent<CharacterStatus>();
        charaAnimation = GetComponent<CharaAnimation>();
        inputManager = FindObjectOfType<InputManager>();
        gameRuleCtrl = FindObjectOfType<GameRuleCtrl>();
        targetCursor = FindObjectOfType<TargetCursor>();
        targetCursor.SetPosition(transform.position);
    }

    // Update is called once per frame
    void Update()
    {
        switch (state)
        {
            case State.Walking:
                Walking();
                break;
            case State.Attacking:
                Attacking();
                break;
        }

        if (state != nextState)
        {
            state = nextState;
            switch (state)
            {
                case State.Walking:
                    WalkStart();
                    break;
                case State.Attacking:
                    AttackStart();
                    break;
                case State.Died:
                    Died();
                    break;
            }
        }
    }


    // 스테이트를 변경한다.
    void ChangeState(State nextState)
    {
        this.nextState = nextState;
    }

    void WalkStart()
    {
        StateStartCommon();
    }

    void Walking()
    {
        if (inputManager.Clicked())
        {
            // RayCast로 대상물을 조사한다.
            Ray ray = Camera.main.ScreenPointToRay(inputManager.GetCursorPosition());
            RaycastHit hitInfo;
            if (Physics.Raycast(ray, out hitInfo, RayCastMaxDistance, (1 << LayerMask.NameToLayer("Ground")) | (1 << LayerMask.NameToLayer("EnemyHit"))))
            {
                // 지면이 클릭되었다.
                if (hitInfo.collider.gameObject.layer == LayerMask.NameToLayer("Ground"))
                {
                    SendMessage("SetDestination", hitInfo.point);
                    targetCursor.SetPosition(hitInfo.point);
                }
                    // 적이 클릭되었다.
                if (hitInfo.collider.gameObject.layer == LayerMask.NameToLayer("EnemyHit"))
                {
                    // 수평 거리를 체크해서 공격할지 결정한다.
                    Vector3 hitPoint = hitInfo.point;
                    hitPoint.y = transform.position.y;
                    float distance = Vector3.Distance(hitPoint, transform.position);
                    if (distance < attackRange)
                    {
                        // 공격.
                        attackTarget = hitInfo.collider.transform;
                        targetCursor.SetPosition(attackTarget.position);
                        ChangeState(State.Attacking);
                    }
                    else
                    {
                        SendMessage("SetDestination", hitInfo.point);
                        targetCursor.SetPosition(hitInfo.point);
                    }
                }
            }
        }
    }

    // 공격 스테이트가 시작되기 전에 호출된다.
    void AttackStart()
    {
        StateStartCommon();
        status.attacking = true;

        // 적 방향으로 돌아보게 한다.
        Vector3 targetDirection = (attackTarget.position - transform.position).normalized;
        SendMessage("SetDirection", targetDirection);

        // 이동을 멈춘다.
        SendMessage("StopMove");
    }

    // 공격 중 처리.
    void Attacking()
    {
        if (charaAnimation.IsAttacked())
            ChangeState(State.Walking);
    }

    void Died()
    {
        status.died = true;
        gameRuleCtrl.GameOver();
    }

    void Damage(AttackArea.AttackInfo attackInfo)
    {
        GameObject effect = Instantiate(hitEffect, transform.position, Quaternion.identity) as GameObject;
        effect.transform.localPosition = transform.position + new Vector3(0.0f, 0.5f, 0.0f);
        Destroy(effect, 0.3f);

        status.HP -= attackInfo.attackPower;
        if (status.HP <= 0)
        {
            status.HP = 0;
            // 체력이 0이므로 사망 스테이트로 전환한다.
            ChangeState(State.Died);
        }
    }

    // 스테이트가 시작되기 전에 스테이터스를 초기화한다.
    void StateStartCommon()
    {
        status.attacking = false;
        status.died = false;
    }
}

