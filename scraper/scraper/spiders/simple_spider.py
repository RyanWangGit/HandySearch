import scrapy
from scrapy.linkextractors import LinkExtractor
from scrapy_splash import SplashRequest
from bs4 import BeautifulSoup
import re


class SimpleSpider(scrapy.Spider):
    name = 'simple'
    start_urls = ['http://news.qq.com/']
    count = 0

    def parse(self, response):
        le = LinkExtractor()
        for link in le.extract_links(response):
            yield SplashRequest(
                link.url,
                self.parse_link,
                endpoint='render.html',
                args={
                    'har': 1,
                    'html': 1
                }
            )

    @staticmethod
    def clean_useless_characters(string):
        return re.sub('[\n\t\|]+|\\u3000|\\xa0|\\u200b', ' ', string)

    @staticmethod
    def trim_white_space(string):
        return re.sub('[\s\u0020\xa0\u3000\u200b]+', ' ', string).strip()

    def parse_link(self, response):
        soup = BeautifulSoup(response.body, "lxml")
        if soup.title is None:
            yield None
        else:
            yield {
                'title': SimpleSpider.clean_useless_characters(soup.title.string),
                'content': SimpleSpider.trim_white_space(' '.join([self.clean_useless_characters(tag.get_text())
                                                                   for tag in soup.find_all(['p', 'li', 'span'])])),
                'url': response.url
            }
