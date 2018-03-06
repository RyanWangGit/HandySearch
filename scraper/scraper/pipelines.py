# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/topics/item-pipeline.html

from scrapy.exceptions import DropItem
import opencc


class ValidationPipeline:
    def process_item(self, item, spider):
        if item is None:
            raise DropItem('Item is None')
        elif None or '' in item.values():
            raise DropItem('Item field not complete')
        elif item['title'].startswith("创建“") and item['title'].endswith("” - 维基百科，自由的百科全书"):
            raise DropItem('Item title contains 创建')
        elif item['title'].startswith('维基百科:') or item['title'].startswith('维基百科讨论:') or \
                item['title'].startswith('编辑“') or item['title'].startswith('Wikipedia:'):
            raise DropItem('Item title contains 维基百科:')
        elif item['title'].startswith('Category:') or item['title'].startswith('Template:') or \
                item['title'].startswith('Template talk:') or \
                item['title'].endswith('”的版本历史 - 维基百科，自由的百科全书') or \
                item['title'].endswith('”的源代码 - 维基百科，自由的百科全书') or \
                item['title'].startswith('Log in - Wikipedia') or item['title'].startswith('模块:') or \
                item['title'].endswith('”的版本间的差异 - 维基百科，自由的百科全书') or \
                item['title'].startswith('创建账户 - 维基百科，自由的百科全书') or \
                item['title'].startswith('User:') or item['title'].startswith('MediaWiki:') or \
                item['title'].startswith('所有页面（') or item['title'].startswith('Portal:'):
            raise DropItem('Item title contains meaningless content')
        else:
            return item
