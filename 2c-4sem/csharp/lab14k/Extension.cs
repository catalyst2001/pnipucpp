using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace lab14k
{
    public static class Extension
    {
        public static int Count(this Queue<Organization> collection, Func<Organization, bool> predicate)
        {
            int count = collection.Where(predicate).Count();
            return count;
        }

        public static string Aggregate(this Queue<Organization> queue, string separator)
        {
            if (queue == null)
            {
                throw new ArgumentNullException(nameof(queue));
            }

            return queue.Select(organization => organization.GetFullInfo()).Aggregate((result, fullInfo) => result + separator + fullInfo);
        }

        public static Queue<Organization> SortByName(this Queue<Organization> queue)
        {
            if (queue == null)
            {
                throw new ArgumentNullException(nameof(queue));
            }

            var sortedList = queue.OrderBy(organization => organization.name).ToList();
            var sortedQueue = new Queue<Organization>(sortedList);
            return sortedQueue;
        }

    }
}
