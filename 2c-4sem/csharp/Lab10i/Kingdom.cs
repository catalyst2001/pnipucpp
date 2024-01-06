using System;

namespace Lab10_1
{
    public class Kingdom: Monarchy
    {
        public Kingdom(string name) : base(name) { }

        public override string GetName()
        {
            return "Королевство " + base.name;
        }
    }
}
