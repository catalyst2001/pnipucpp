using System;
using System.Security.Cryptography;

namespace lab12i
{
    public class Republic: State
    {
        public State BaseState
        {
            get
            {
                return new State(name, governmentForm);
            }
        }

        public Republic(string name) : base(name, "Республика"){}

        public override void SetGovernmentForm(string governmentForm)
        {
            return;
        }
    }
}
