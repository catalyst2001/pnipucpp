using System;
using System.Xml.Linq;

namespace Lab10_1
{
    public class State: IRandomInit
    {
        protected string name;
        protected string governmentForm;
        private int _random;
        int IRandomInit.random => _random;

        public State(string name, string governmentForm) {
            this.name = name;
            this.governmentForm = governmentForm;
        }

        public virtual string GetName()
        {
            return this.name;
        }

        public virtual void SetName(string name)
        {
            this.name = name;
        }

        public virtual string GetGovernmentForm()
        {
            return this.governmentForm;
        }

        public virtual void SetGovernmentForm(string governmentForm)
        {
            this.governmentForm = governmentForm;
        }

        public void RandomInit()
        {
            _random = new Random().Next(0, 100);
        }

        public State ShallowCopy()
        {
            return (State)this.MemberwiseClone();
        }

        public object Clone()
        {
            return new State(this.name, this.governmentForm);
        }

        public int CompareTo(object? obj)
        {
            if (((IRandomInit)obj).random == this._random)
                return 0;
            if (this._random > ((IRandomInit)obj).random)
                return 1;
            return -1;
        }

        public int Compare(object? x, object? y)
        {
            if (((IRandomInit)x).random == ((IRandomInit)y).random)
                return 0;
            if (((IRandomInit)x).random > ((IRandomInit)y).random)
                return 1;
            return -1;
        }
    }
}
