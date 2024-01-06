using System;

namespace Lab10_1
{
    public class Monarchy: State
    {
        public Monarchy(string name) : base(name, "Монархия") { }

        public override void SetGovernmentForm(string governmentForm)
        {
            return;
        }
    }
}
