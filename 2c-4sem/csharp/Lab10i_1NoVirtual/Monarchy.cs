using System;

namespace Lab10_1NoVirtual
{
    public class Monarchy: State
    {
        public Monarchy(string name) : base(name, "Монархия") { }

        public void SetGovernmentForm(string governmentForm)
        {
            return;
        }
    }
}
