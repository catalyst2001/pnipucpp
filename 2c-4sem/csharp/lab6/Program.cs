using System;
using System.Runtime.CompilerServices;
using System.Text.RegularExpressions;

namespace lab6
{
    public class Program
    {
        public static char[] ConsoleReadString()
        {
            string str;
            while(true)
            {
                try
                {
                    Console.Write("Pease write line: ");
                    str = Console.ReadLine();
                    if (str == null)
                        continue;
                }
                catch
                {
                    continue;
                }
                break;
            }
            Console.WriteLine("Your string is: {0}", str);
            return str.ToArray();
        }

        public static bool IsVowel(char sym)
        {
            char[] syms = {
                'a', 'e', 'i', 'o', 'u',
                'а', 'е', 'ё', 'и', 'о', 'у', 'ы', 'э', 'ю', 'я'
            };

            for (int i = 0; i < syms.Length; i++)
                if(char.ToLower(sym) == syms[i])
                    return true;

            return false;
        }

        public static void RemoveFromArray<T>(ref T[] array, int from_idx, int count)
        {
            int j = 0;
            T[] copy = new T[array.Length - count];
            for (int i = 0; i < from_idx; i++)
                copy[j++] = array[i];

            for (int i = from_idx + count; i < array.Length; i++)
                copy[j++] = array[i];

            array = copy;
        }

        public static void RemoveLastVowel(ref char[] array)
        {
            int lastVowelIdx = -1;
            for (int i = array.Length - 1; i > 0; i--)
            {
                if (IsVowel(array[i]))
                {
                    lastVowelIdx = i;
                    break;
                }
            }

            if (lastVowelIdx == -1)
                return;

            RemoveFromArray<char>(ref array, lastVowelIdx, 1);
        }

        public static void SwapFirstAndLastSentence(ref char[] array)
        {
            char[] delims = new char[] { '.', '!', '?' };

            string[] sentences = new string(array).Split(delims, StringSplitOptions.None);
            string temp = sentences[0];
            sentences[0] = sentences[sentences.Length - 1];
            sentences[sentences.Length - 1] = temp;
            string str = string.Concat(sentences);
            //Console.WriteLine("SwapFirstAndLastSentence end: " + str + "  Len:" + sentences.Length);
            str.CopyTo(array);
        }

        public static int StrChrs(string str, string delims, int startpos = 0)
        {
            for (int i = startpos; i < str.Length; i++)
            {
                for (int j = 0; j < delims.Length; j++)
                {
                    if (str[i] == delims[j])
                    {
                        return i;
                    }
                }
            }
            return -1;
        }

        public static int StrrChrs(string str, string delims, int endpos = -1)
        {
            if (endpos == -1)
                endpos = str.Length - 1;

            for (int i = endpos; i > 0; i--)
            {
                for (int j = 0; j < delims.Length; j++)
                {
                    if (str[i] == delims[j])
                    {
                        return i;
                    }
                }
            }
            return -1;
        }

        public static bool IsPrint(char chr)
        {
            return char.IsLetterOrDigit(chr) || char.IsPunctuation(chr);
        }

        public static void SwapFirstAndLastSentence2(ref char[] array)
        {
            int first_delim, last_delim;
            string text = new string(array).Trim();

            first_delim = StrChrs(text, ".!?");  // find fist delim
            last_delim = StrrChrs(text, ".!?");  // find last delim

            //Console.WriteLine("IN({0})  first_delim={1}  last_delim={2}", text, first_delim, last_delim);
            //Console.WriteLine("text.Length={0} last_delim={1}", text.Length, last_delim);
            if ((text.Length - 1) == last_delim)
            {
                //Console.WriteLine("text.Length == last_delim");
                last_delim = StrrChrs(text, ".!?", last_delim - 1);
            }

            //Console.WriteLine("last_delim POST={0}", last_delim);
            if (first_delim == -1 || last_delim == -1)
                return;

            last_delim++;
            while (!IsPrint(text[last_delim]))
                last_delim++;

            first_delim++;
            while (!IsPrint(text[first_delim]))
                first_delim++;

            int length = text.Length - last_delim;
            if (length < 1)
                return;

            int mlength = last_delim - first_delim;
            if (mlength < 1)
                return;

            new string(text.Substring(last_delim, length) + text.Substring(first_delim, mlength) + text.Substring(0, first_delim)).ToArray().CopyTo(array, 0);
            //Console.WriteLine("END_TEXT: " + end);
        }

        static void PrintMenuItems()
        {
            Console.WriteLine("\n\n1. remove last vowel");
            Console.WriteLine("2. swap first and last sentence");
            Console.WriteLine("\n0. Exit\n\n");
        }

        static void Main(string[] args)
        {
            char[] array;
            bool bcontinue = true;
            do
            {
                PrintMenuItems();
                try
                {
                    Console.Write(">> ");
                    int item = Convert.ToInt32(Console.ReadLine());
                    switch (item)
                    {
                        case 1:
                            array = ConsoleReadString();
                            RemoveLastVowel(ref array);
                            Console.WriteLine("String with removed last vowel: " + new string(array));
                            break;

                        case 2:
                            array = ConsoleReadString();
                            SwapFirstAndLastSentence2(ref array);
                            Console.WriteLine("\n\nSTR: " + new string(array));
                            break;

                        case 0:
                            bcontinue = false;
                            continue;

                        default:
                            Console.WriteLine("Unknown menu item!");
                            break;
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine("EXCEPTION: " + e.ToString());
                }
            } while (bcontinue);
        }
    }
}