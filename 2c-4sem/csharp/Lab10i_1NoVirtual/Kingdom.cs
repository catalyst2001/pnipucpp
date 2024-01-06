using System;

namespace Lab10_1NoVirtual
{
    public class Kingdom: Monarchy
    {
        public Kingdom(string name) : base(name) { }

        public string GetName()
        {
            return "Королевство " + base.name;
        }
    }
}
