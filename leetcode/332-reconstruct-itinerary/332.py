#!/usr/bin/env python3


class Solution:
    def findItinerary(self, tickets):
        """
        :type tickets: List[List[str]]
        :rtype: List[str]
        """

        graph = {}
        for ticket in tickets:
            if ticket[0] not in graph:
                graph[ticket[0]] = []
            graph[ticket[0]].append(ticket[1])
        for to_airports in graph.values():
            to_airports.sort()  # Sort it in-place.

        def solve(from_airport, num_edges, itinerary):
            if num_edges == 0:
                return True
            to_airports = graph.get(from_airport)
            if to_airports is None:
                return False
            for i in range(len(to_airports)):
                to_airport = to_airports[i]
                if to_airport is None:
                    continue
                # For convenience, modify to_airports in-place.
                to_airports[i] = None
                itinerary.append(to_airport)
                if solve(to_airport, num_edges - 1, itinerary):
                    return True
                itinerary.pop()
                to_airports[i] = to_airport
            return False

        itinerary = ['JFK']
        okay = solve(itinerary[0], len(tickets), itinerary)
        assert okay  # The problem guarantees a valid itinerary.

        return itinerary


if __name__ == '__main__':
    solution = Solution()
    print(solution.findItinerary([
        ['MUC', 'LHR'],
        ['JFK', 'MUC'],
        ['SFO', 'SJC'],
        ['LHR', 'SFO'],
    ]))
    print(solution.findItinerary([
        ['JFK', 'SFO'],
        ['JFK', 'ATL'],
        ['SFO', 'ATL'],
        ['ATL', 'JFK'],
        ['ATL', 'SFO']
    ]))
