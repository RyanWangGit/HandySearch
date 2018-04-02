"""
Item Exporters are used to export/serialize items into sqlite3 database.
"""

from scrapy.exporters import BaseItemExporter
import sqlite3


class SqliteWebpageExporter(BaseItemExporter):
    def __init__(self, file, **kwargs):
        self._configure(kwargs)
        self.conn = sqlite3.connect(file.name)
        self.conn.execute(r"""CREATE TABLE IF NOT EXISTS `webpages`(
                                                    `id` INTEGER PRIMARY KEY,
                                                    `title` VARCHAR DEFAULT NULL,
                                                    `content` VARCHAR DEFAULT NULL,
                                                    `url` VARCHAR DEFAULT NULL UNIQUE 
                                                  );
                                             """)
        self.conn.commit()
        self.conn.text_factory = str

    def export_item(self, item):
        self.start_exporting()
        self.conn.execute(r"""INSERT INTO webpages(title, content, url) VALUES ('%s', '%s', '%s')""" %
                          (item['title'], item['content'], item['url']))
        self.conn.commit()
        self.finish_exporting()

    def __del__(self):
        self.conn.close()
