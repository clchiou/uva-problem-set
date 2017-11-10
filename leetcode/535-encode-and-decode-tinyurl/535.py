#
# 535 should really be be folded into "534. Design TinyURL"; otherwise
# this cheated solution is totally valid but useless nonetheless.
#


class Codec:

    def encode(self, longUrl):
        """Encodes a URL to a shortened URL.

        :type longUrl: str
        :rtype: str
        """
        return longUrl

    def decode(self, shortUrl):
        """Decodes a shortened URL to its original URL.

        :type shortUrl: str
        :rtype: str
        """
        return shortUrl
