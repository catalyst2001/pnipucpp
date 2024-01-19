using System;

namespace lab11i
{
    public class Kingdom: Monarchy
    {
        public Kingdom(string name) : base(name) { }

        public State BaseState
        {
            get
            {
                return new State(name, governmentForm);
            }
        }

        public override string GetName()
        {
            return "Королевство " + base.name;
        }
    }
}
