using lab13i;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13i
{
    public class Journal
    {
        List<JournalEntry> journal;

        public Journal()
        {
            journal = new List<JournalEntry>();
        }

        public void CollectionCountChanged(object sender, CollectionHandlerEventArgs e)
        {
            Console.WriteLine($"Collection '{e.NameCollection}' count changed.");
            JournalEntry je = new JournalEntry(e.NameCollection, e.ChangeCollection, e.Obj.ToString());
            journal.Add(je);
        }

        public void CollectionReferenceChanged(object sender, CollectionHandlerEventArgs e)
        {
            Console.WriteLine($"Reference changed in collection '{e.NameCollection}'.");
            JournalEntry je = new JournalEntry(e.NameCollection, e.ChangeCollection, e.Obj.ToString());
            journal.Add(je);
        }

        public void PrintJournal()
        {
            Console.WriteLine("------- Printing journal -------");
            foreach (JournalEntry je in journal)
            {
                Console.WriteLine("Name collection: {0}  Event name: {1}", je.NameCollection, je.EventName);
            }
        }
    }
}
