using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;

namespace _0209cs
{
    class Program
    {
        const int bits = 6;
        const long numBits = (1 << bits);
        const long mask = numBits - 1;
        static ConcurrentDictionary<(long, bool, bool), long> cache = new ConcurrentDictionary<(long, bool, bool), long>();

        static long CountMatchingBitPatterns(long numBits, bool firstBitOfAll, bool firstBit)
        {
            return cache.GetOrAdd((numBits, firstBitOfAll, firstBit), CountMatchingBitPatterns);
        }

        static long CountMatchingBitPatterns((long numBits, bool firstBitOfAll, bool firstBit) p)
        {
            long retval;
            if (p.numBits == 1)
            {
                retval = p.firstBit && p.firstBitOfAll ? 0 : 1;
            }
            else
            {
                retval = 0;
                retval += CountMatchingBitPatterns(p.numBits - 1, p.firstBitOfAll, !p.firstBit);
                if (!p.firstBit)
                {
                    retval += CountMatchingBitPatterns(p.numBits - 1, p.firstBitOfAll, p.firstBit);
                }
            }
            return retval;
        }

        static long CountMatchingBitPatterns(long numBits)
        {
            long retval =
                CountMatchingBitPatterns(numBits, false, false) +
                CountMatchingBitPatterns(numBits, true, true);
            return retval;
        }


        static long NextBitInSequence(long n)
        {
            long a = (n >> (bits - 1)) & 1;
            long b = (n >> (bits - 2)) & 1;
            long c = (n >> (bits - 3)) & 1;
            long next = ((n << 1) & mask) | (a ^ (b & c));
            return next;
        }

        static void Main(string[] args)
        {
            var bitSequences = new List<List<long>>();
            for(long i = 0; bitSequences.Sum(b => b.Count) < numBits; i++)
            {
                if(!bitSequences.Any(l => l.Contains(i)))
                {
                    var bitSequence = new List<long>();
                    for(long n = i; !bitSequence.Contains(n); n = NextBitInSequence(n))
                    {
                        bitSequence.Add(n);
                    }
                    bitSequences.Add(bitSequence);
                }
            }

            var bitSequenceLengths = bitSequences.Select(b => b.Count);
            var bitsInSequence = bitSequenceLengths.Select(b => CountMatchingBitPatterns(b));
            var totalCombinations = bitsInSequence.Aggregate(1L, (p, e) => p * e);
            Console.WriteLine(totalCombinations);
        }
    }
}
