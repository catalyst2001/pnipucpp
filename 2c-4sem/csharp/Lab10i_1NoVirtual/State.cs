using System;
using System.Xml.Linq;

namespace Lab10_1NoVirtual
{
    public class State
    {
        protected string name;
        protected string governmentForm;

        public State(string name, string governmentForm) {
            this.name = name;
            this.governmentForm = governmentForm;
        }

        public string GetName()
        {
            return this.name;
        }

        public void SetName(string name)
        {
            this.name = name;
        }

        public string GetGovernmentForm()
        {
            return this.governmentForm;
        }

        public void SetGovernmentForm(string governmentForm)
        {
            this.governmentForm = governmentForm;
        }
    }
}
