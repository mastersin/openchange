# -*- coding: utf-8 -*-
#
# Copyright (C) 2014 Enrique J. Hernández <ejhernandez@zentyal.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
"""
Wrapper over LDB connection to reconnect when the connection has failed
"""
import logging


from ldb import LdbError, ERR_OPERATIONS_ERROR
from samba.samdb import SamDB


log = logging.getLogger(__name__)


class SamDBWrapper(SamDB):
    """Class to wrap the connection to reconnect when the connection is
    lost in a search.
    """
    def search(self, *args, **kwargs):
        """Override to perform the SamDB.search and if it fails because of a
        reconnection, then it tries to reconnect using the same
        parameters.

        See SamDB.search for parameters description.
        """
        try:
            return super(SamDBWrapper, self).search(*args, **kwargs)
        except LdbError as [num, msg]:
            # We'd like to retry on operations error
            # Maybe the daemon closed the connection
            if num == ERR_OPERATIONS_ERROR:
                log.warn('Trying to reconnect after %s' % msg)
                self.connect(url=self.url)
            else:
                # Re-raise the original exception
                raise
        return super(SamDBWrapper, self).search(*args, **kwargs)
