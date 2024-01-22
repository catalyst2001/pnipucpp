using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace lab14i
{
    public static class Extension
    {
        public static int Count(this Dictionary<string, State> collection, Func<KeyValuePair<string, State>, bool> predicate)
        {
            int count = collection.Where(predicate).Count();
            return count;
        }

        public static string Aggregate(this Dictionary<string, State> dictionary, string separator)
        {
            if (dictionary == null)
            {
                throw new ArgumentNullException(nameof(dictionary));
            }

            return dictionary.Keys.Aggregate((result, key) => result + separator + key);
        }
    }
}
