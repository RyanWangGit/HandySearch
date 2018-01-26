import scrapy
from scrapy.linkextractors.sgml import SgmlLinkExtractor
from scrapy.spiders import Rule
import re


class WikiSpider(scrapy.Spider):
    name = 'wiki'
    allowed_domains = ['wikipedia.org']
    start_urls = ['https://zh.wikipedia.org/wiki/Wikipedia:%E9%A6%96%E9%A1%B5']
    rules = (
        Rule(SgmlLinkExtractor(restrict_xpaths='//div[@class="mw-body-content"]//a/@href'))
    )

    def parse(self, response):
        yield {
            'title': response.xpath('//title/text()').extract_first(),
            'content': WikiSpider.clean_string(
                ''.join(response.xpath('//div[@id=\'mw-content-text\']')
                        .xpath('//p/text()|//li/text()|//span/text()|//a/text()').extract())),
            'url': response.url
        }

    @staticmethod
    def clean_string(string):
        return re.sub('[\s\u0020\xa0\u3000\u200b]+', ' ',
                      re.sub('[\n\t\|]+|\\u3000|\\xa0|\\u200b', ' ', string)).strip().replace('\'', '\'\'')
