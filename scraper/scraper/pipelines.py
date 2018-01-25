# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/topics/item-pipeline.html

import sqlite3
from scrapy.exceptions import DropItem


class ValidationPipeline:
    def process_item(self, item, spider):
        if item is None:
            raise DropItem('Item is None')
        elif None or '' in item.values():
            raise DropItem('Item field not complete')
        else:
            return item
