#!/usr/bin/env python3


class Solution:
    def validUtf8(self, data):
        """
        :type data: List[int]
        :rtype: bool
        """

        def check(data):
            if not data:
                return False, -1

            # Check for one-byte character.
            if (data[0] & 0b10000000) == 0 and len(data) >= 1:
                return True, 1

            if (data[0] & 0b11100000) == 0b11000000:
                n = 2
            elif (data[0] & 0b11110000) == 0b11100000:
                n = 3
            elif (data[0] & 0b11111000) == 0b11110000:
                n = 4
            else:
                return False, -1

            if len(data) < n:
                return False, -1

            for b in data[1:n]:
                if (b & 0b11000000) != 0b10000000:
                    return False, -1
            return True, n

        offset = 0
        while offset < len(data):
            okay, n = check(data[offset:])
            if not okay:
                return False
            offset += n

        return True


if __name__ == '__main__':
    solution = Solution()
    assert solution.validUtf8([197, 130, 1])
    assert not solution.validUtf8([235, 140, 4])
    assert solution.validUtf8(list('你好世界'.encode('utf8')))
    assert solution.validUtf8([228, 189, 160, 229, 165, 189, 13, 10])
