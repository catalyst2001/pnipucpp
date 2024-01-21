using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12i
{
    public class ConInFormat
    {
        public enum FORMAT_TYPE
        {
            FORMAT_STRING = 0,
            FORMAT_INT16,
            FORMAT_INT32,
            FORMAT_INT64,
            FORMAT_FLOAT,
            FORMAT_DOUBLE
        };

        static protected object ReadLineWithDescription(FORMAT_TYPE format_type, string? description)
        {
            string ret = "empty";
            string? strref = null;
            do {
                Console.Write("{0}: ", description);
                strref = Console.ReadLine();
                if (strref == null) {
                    Console.WriteLine("Reading from input stream failed");
                    return ret;
                }

                ret = strref;
                try {
                    switch (format_type) {
                        case FORMAT_TYPE.FORMAT_INT16:
                            return Convert.ToInt16(ret);

                        case FORMAT_TYPE.FORMAT_INT32:
                            return Convert.ToInt32(ret);

                        case FORMAT_TYPE.FORMAT_INT64:
                            return Convert.ToInt64(ret);

                        case FORMAT_TYPE.FORMAT_FLOAT:
                            return Convert.ToSingle(ret);

                        case FORMAT_TYPE.FORMAT_DOUBLE:
                            return Convert.ToDouble(ret);

                        case FORMAT_TYPE.FORMAT_STRING:
                        default:
                            return Convert.ToString(ret); //default convert to string
                    }
                }
                catch(FormatException) {
                    Console.WriteLine("Exception: format exception occurred. Retype data!");
                    continue;
                }
            } while (true);
        }
    }
}
