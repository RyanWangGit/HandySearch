import scrapy

from scrapy.linkextractors import LinkExtractor
import re


class WikiSpider(scrapy.Spider):
    name = 'wiki'
    allowed_domains = ['zh.wikipedia.org']
    start_urls = ['https://zh.wikipedia.org/wiki/Wikipedia:%E9%A6%96%E9%A1%B5']

    def __init__(self):
        self.le = LinkExtractor(restrict_xpaths='//div[@class="mw-body-content"]')

    def parse(self, response):
        yield {
            'title': response.xpath('//title/text()').extract_first(),
            'content': WikiSpider.clean_string(
                ''.join(response.xpath('//div[@id=\'mw-content-text\']')
                        .xpath('//p/text()|//li/text()|//span/text()|//a/text()').extract())),
            'url': response.url
        }

        for link in self.le.extract_links(response):
            yield scrapy.Request(link.url, callback=self.parse)

    @staticmethod
    def clean_string(string):
        return re.sub('[\s\u0020\xa0\u3000\u200b]+', ' ',
                      re.sub('[\n\t\|]+|\\u3000|\\xa0|\\u200b', ' ', string)).strip().replace('\'', '\'\'')
