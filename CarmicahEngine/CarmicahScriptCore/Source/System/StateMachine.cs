using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class StateMachine
    {
        Dictionary<string, State> m_stateMap = new Dictionary<string, State>();
        State m_currState;
        State m_nextState;
        // Start is called before the first frame update
        public StateMachine()
        {
            m_currState = null;
            m_nextState = null;
        }

        // Update is called once per frame
        public void Update(ref StateMachine sm)
        {
            if (m_currState == null && m_nextState != null)
            {   // m_currState is null 
                m_currState = m_nextState;
                m_currState.Enter();
            }
            else if (m_nextState != m_currState)
            { // There is a change
                m_currState.Exit();
                m_currState = m_nextState;
                m_currState.Enter();
            }
            m_currState.Update(ref sm);
        }

        public void AddState(State newState)
        {
            if (newState == null)
                return;
            if (m_stateMap.ContainsKey(newState.m_stateID_))
                return; // it is in the list

            //if (m_currState == null)
            //{
            //    m_currState = m_nextState = newState; // Set the states to thefirst state passed in
            //    m_currState.Enter();
            //}

            m_stateMap.Add(newState.m_stateID_, newState);
        }

        public void SetNextState(string nextStateID)
        {
            //State nextState;
            m_stateMap.TryGetValue(nextStateID, out m_nextState);

        }

        public string GetCurrentState()
        {
            if (m_currState != null)
                return m_currState.m_stateID_;
            return "<No States>";
        }

    }


    public class State
    {
        public string m_stateID_;

        public State(string _m_stateID)
        {
            m_stateID_ = _m_stateID;
        }

        // Start is called before the first frame update
        public virtual void Enter()
        {

        }

        // Update is called once per frame
        public virtual void Update(ref StateMachine sm)
        {

        }

        public virtual void Exit()
        {

        }

        //protected string GetStateID()
        //{

        //}
    }

}
