# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/topics/item-pipeline.html

from scrapy.exceptions import DropItem


class ValidationPipeline:
    def process_item(self, item, spider):
        if item is None:
            raise DropItem('Item is None')
        elif None or '' in item.values():
            raise DropItem('Item field not complete')
        elif item['title'].startswith("创建“") and item['title'].endswith("” - 维基百科，自由的百科全书"):
            raise DropItem('Item title contains 创建')
        elif item['title'].startswith('维基百科:'):
            raise DropItem('Item title contains 维基百科:')
        elif 'Category:' in item['title']:
            raise DropItem('Item title contains category:')
        else:
            return item
