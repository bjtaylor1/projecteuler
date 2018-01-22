using System;
using System.Collections;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace euler480n
{
    class Program
    {
        private const int LengthLimit = 15;
        private const string Phrase = "thereisasyetinsufficientdataforameaningfulanswer";

        private static readonly ConcurrentDictionary<CacheKey, long> StartingWithCache = new ConcurrentDictionary<CacheKey, long>();

        private static long StartingWith(string word, ICollection<char> remaining)
        {
            var cacheKey = new CacheKey(word, remaining);
            return StartingWithCache.GetOrAdd(cacheKey, CalculateStartingWith);
        }

        private static long CalculateStartingWith(CacheKey k)
        {
            long res = 1;
            if (k.Word.Length < LengthLimit)
            {
                foreach (var c in k.Remaining.Distinct().ToArray())
                {
                    if (k.Remaining.Remove(c))
                    {
                        res += StartingWith(k.Word + c, k.Remaining);
                        k.Remaining.Add(c);
                    }
                }
            }
            return res;

        }

        private static long StartingWith(string word)
        {
            var remaining = Phrase.ToList();
            foreach (var c in word)
            {
                if (!remaining.Remove(c)) return 0;
            }
            return StartingWith(word, remaining);
        }

        private static long N(string word)
        {
            long result = 0;
            for (int pos = word.Length - 1; pos >= 0; pos--)
            {
                var prefix = word.Substring(0, pos);
                result++;
                foreach (var c in Phrase.Distinct().Where(c => c < word[pos]).ToArray())
                {
                    result += StartingWith(prefix + c);
                }
             }
            return result;
        }

        private static void test(string word)
        {
            Console.WriteLine($"N({word}) = {N(word)}");
        }

        static void Main(string[] args)
        {
            Console.Out.WriteLine(N("aaaaaacdeeeeeef"));
            Console.Out.WriteLine(N("aaaaaacdeeeeeey"));
            Console.Out.WriteLine(N("euleoywuttttsss"));
            Console.Out.WriteLine(N("euler"));
        }
    }

    public class CacheKey
    {
        public string Word { get; }
        public ICollection<char> Remaining { get; }
        public string RemainingKey { get; }

        public CacheKey(string word, ICollection<char> remaining)
        {
            Word = word;
            Remaining = remaining;
            RemainingKey = remaining.GroupBy(c => c).Select(g => g.Count()).OrderBy(c => c)
                .Aggregate(new StringBuilder(), (sb, i) => sb.Append(i)).ToString();
        }

        public override bool Equals(object obj)
        {
            return obj is CacheKey key &&
                   RemainingKey == key.RemainingKey;
        }

        public override int GetHashCode()
        {
            return -1573574710 + EqualityComparer<string>.Default.GetHashCode(RemainingKey);
        }
    }

}
