using System;
using System.Collections.Generic;

namespace NewYearsBallTools
{
    class Led : IComparable<Led>
    {
        public int index { get; set; }
        public List<Led> neighbors { get; set; } = new List<Led>();

        public int CompareTo(Led other)
        {
            return index - other.index;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            var leds = LoadNeighborsFile();

            List<Led> pentagonPixels = new List<Led>();
            foreach (var led in leds)
            {
                if (isInPentagon(leds, led.index))
                {
                    pentagonPixels.Add(led);
                }
            }

            ICollection<ICollection<ICollection<Led>>> pentagonRings = getPentagonRings(leds);
            foreach (var axis in pentagonRings)
            {
                Console.WriteLine("Axis");
                int axisCount = 0;
                foreach (var ring in axis)
                {
                    Console.Write("Ring: ");
                    Console.Write($" {ring.Count}\n");
                    axisCount += ring.Count;
                    int ringCount = 0;
                    foreach (var pixel in ring)
                    {
                        Console.Write($"{pixel.index}\t");
                        ringCount++;
                    }
                    Console.Write("\n");
                }
                Console.Write($"AxisCount: {axisCount}\n");
            }
        }

        static List<Led> LoadNeighborsFile()
        {
            List<Led> leds = new List<Led>();
            for (int i = 0; i < 180; i++)
            {
                leds.Add(new Led() { index = i });
            }

            string fileContent = System.IO.File.ReadAllText(@"D:\DEV\Arduino\featherHuzzah_NewYearsBall_v2\data\ledNeighbors.tsv");
            foreach (var line in fileContent.Split('\n'))
            {
                var numbers = line.Split('\t');
                leds[int.Parse(numbers[0])].neighbors = new List<Led>() 
                { 
                    leds[int.Parse(numbers[1])],
                    leds[int.Parse(numbers[2])],
                    leds[int.Parse(numbers[3])] 
                };
            }

            return leds;
        }

        static bool isInPentagon(List<Led> leds, int index)
        {
            List<int> neighborList;
            foreach (var firstNeighbor in leds.Find(l => l.index == index).neighbors)
            {
                foreach (var secondNeighbor in leds.Find(l => l == firstNeighbor).neighbors)
                {
                    foreach (var thirdNeighbor in leds.Find(l => l == secondNeighbor).neighbors)
                    {
                        foreach (var forthNeighbor in leds.Find(l => l == thirdNeighbor).neighbors)
                        {
                            foreach (var fifthNeighbor in leds.Find(l => l == forthNeighbor).neighbors)
                            {
                                if (fifthNeighbor.index == index)
                                {
                                    var pentagonLeds = new List<Led>() { leds[index], firstNeighbor, secondNeighbor, thirdNeighbor, forthNeighbor };
                                    pentagonLeds.Sort();
                                    //Console.WriteLine($"{pentagonLeds[0]}\t{pentagonLeds[1]}\t{pentagonLeds[2]}\t{pentagonLeds[3]}\t{pentagonLeds[4]}");
                                    return true;
                                }
                            }
                        }
                    }
                }
            }

            return false;
        }

        static ICollection<ICollection<ICollection<Led>>> getPentagonRings(List<Led> leds)
        {
            List<ICollection<ICollection<Led>>> pentagonRings = new List<ICollection<ICollection<Led>>>();
            List<List<int>> pentagons = new List<List<int>>()
            {
                new List<int>() { 5, 4, 3, 32, 33 },
                new List<int>() { 11, 10, 9, 37, 38 },
                new List<int>() { 17, 16, 15, 42, 43 },
                new List<int>() { 23, 22, 21, 47, 48 },
                new List<int>() { 29, 28, 27, 52, 53 },
                new List<int>() { 74, 73, 72, 71, 70 },
                new List<int>() { 106, 107, 108, 135, 159 },
                new List<int>() { 112, 113, 114, 140, 139 },
                new List<int>() { 118, 119, 120, 145, 144 },
                new List<int>() { 124, 125, 126, 150, 149 },
                new List<int>() { 130, 131, 132, 155, 154 },
                new List<int>() { 175, 176, 177, 178, 179 }
            };

            foreach (var pentagonList in pentagons)
            {
                List<ICollection<Led>> axis = new List<ICollection<Led>>();
                HashSet<Led> knownAxisLeds = new HashSet<Led>();

                List<Led> pentagon = new List<Led>()
                {
                    leds[pentagonList[0]],
                    leds[pentagonList[1]],
                    leds[pentagonList[2]],
                    leds[pentagonList[3]],
                    leds[pentagonList[4]]
                };
                axis.Add(pentagon);
                knownAxisLeds.Add(pentagon[0]);
                knownAxisLeds.Add(pentagon[1]);
                knownAxisLeds.Add(pentagon[2]);
                knownAxisLeds.Add(pentagon[3]);
                knownAxisLeds.Add(pentagon[4]);

                HashSet<Led> ring2 = new HashSet<Led>();
                foreach (Led pixel in pentagon)
                {
                    foreach (Led neighbor in pixel.neighbors)
                    {
                        if (knownAxisLeds.Add(neighbor))
                        {
                            ring2.Add(neighbor);
                        }

                        foreach (Led secondNeighbor in neighbor.neighbors)
                        {
                            if (knownAxisLeds.Add(secondNeighbor))
                            {
                                ring2.Add(secondNeighbor);
                            }
                        }
                    }
                }
                axis.Add(ring2);

                HashSet<Led> ring3 = new HashSet<Led>();
                foreach (Led pixel in ring2)
                {
                    foreach (Led neighbor in pixel.neighbors)
                    {
                        if (knownAxisLeds.Add(neighbor))
                        {
                            ring3.Add(neighbor);
                        }

                        foreach (Led secondNeighbor in neighbor.neighbors)
                        {
                            if (knownAxisLeds.Add(secondNeighbor))
                            {
                                ring3.Add(secondNeighbor);
                            }
                        }
                    }
                }
                axis.Add(ring3);

                HashSet<Led> ring4 = new HashSet<Led>();
                foreach (Led pixel in ring3)
                {
                    foreach (Led neighbor in pixel.neighbors)
                    {
                        if (knownAxisLeds.Add(neighbor))
                        {
                            ring4.Add(neighbor);
                        }

                        foreach (Led secondNeighbor in neighbor.neighbors)
                        {
                            if (knownAxisLeds.Add(secondNeighbor))
                            {
                                ring4.Add(secondNeighbor);
                            }
                        }
                    }
                }
                axis.Add(ring4);

                HashSet<Led> ring5 = new HashSet<Led>();
                foreach (Led pixel in ring4)
                {
                    foreach (Led neighbor in pixel.neighbors)
                    {
                        if (knownAxisLeds.Add(neighbor))
                        {
                            ring5.Add(neighbor);
                        }

                        foreach (Led secondNeighbor in neighbor.neighbors)
                        {
                            if (knownAxisLeds.Add(secondNeighbor))
                            {
                                ring5.Add(secondNeighbor);
                            }
                        }
                    }
                }
                axis.Add(ring5);

                HashSet<Led> ring6 = new HashSet<Led>();
                foreach (Led pixel in ring5)
                {
                    foreach (Led neighbor in pixel.neighbors)
                    {
                        if (knownAxisLeds.Add(neighbor))
                        {
                            ring6.Add(neighbor);
                        }

                        foreach (Led secondNeighbor in neighbor.neighbors)
                        {
                            if (knownAxisLeds.Add(secondNeighbor))
                            {
                                ring6.Add(secondNeighbor);
                            }
                        }
                    }
                }
                axis.Add(ring6);

                HashSet<Led> ring7 = new HashSet<Led>();
                foreach (Led pixel in ring6)
                {
                    foreach (Led neighbor in pixel.neighbors)
                    {
                        if (knownAxisLeds.Add(neighbor))
                        {
                            ring7.Add(neighbor);
                        }

                        foreach (Led secondNeighbor in neighbor.neighbors)
                        {
                            if (knownAxisLeds.Add(secondNeighbor))
                            {
                                ring7.Add(secondNeighbor);
                            }
                        }
                    }
                }
                axis.Add(ring7);

                HashSet<Led> ring8 = new HashSet<Led>();
                foreach (Led pixel in ring7)
                {
                    foreach (Led neighbor in pixel.neighbors)
                    {
                        if (knownAxisLeds.Add(neighbor))
                        {
                            ring8.Add(neighbor);
                        }

                        foreach (Led secondNeighbor in neighbor.neighbors)
                        {
                            if (knownAxisLeds.Add(secondNeighbor))
                            {
                                ring8.Add(secondNeighbor);
                            }
                        }
                    }
                }
                axis.Add(ring8);

                HashSet<Led> ring9 = new HashSet<Led>();
                foreach (Led pixel in ring8)
                {
                    foreach (Led neighbor in pixel.neighbors)
                    {
                        if (knownAxisLeds.Add(neighbor))
                        {
                            ring9.Add(neighbor);
                        }

                        foreach (Led secondNeighbor in neighbor.neighbors)
                        {
                            if (knownAxisLeds.Add(secondNeighbor))
                            {
                                ring9.Add(secondNeighbor);
                            }
                        }
                    }
                }
                axis.Add(ring9);
                
                pentagonRings.Add(axis);
            }

            return pentagonRings;
        }
    }
}
