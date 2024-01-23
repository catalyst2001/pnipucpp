using System;

namespace lab13i
{
    public class Monarchy: State
    {
        public State BaseState
        {
            get
            {
                return new State(name, governmentForm);
            }
        }

        public Monarchy(string name) : base(name, "Монархия") { }

        public override void SetGovernmentForm(string governmentForm)
        {
            return;
        }
    }
}
