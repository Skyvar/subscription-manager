/**
 * Copyright (c) 2018 Red Hat, Inc.
 *
 * This software is licensed to you under the GNU General Public License,
 * version 2 (GPLv2). There is NO WARRANTY for this software, express or
 * implied, including the implied warranties of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. You should have received a copy of GPLv2
 * along with this software; if not, see
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
 *
 * Red Hat trademarks are not licensed under GPLv2. No permission is
 * granted to use or replicate Red Hat trademarks that are incorporated
 * in this software or its documentation.
 */

#include <glib.h>
#include <stdio.h>
#include "productdb.h"

typedef struct {
    ProductDb *db;
} dbFixture;

void setup(dbFixture *fixture, gconstpointer testData) {
    fixture->db = initProductDb();
}

void teardown(dbFixture *fixture, gconstpointer testData) {
    freeProductDb(fixture->db);
}

void testAdd(dbFixture *fixture, gconstpointer ignored) {
    ProductDb *db = fixture->db;
    db->path = "testing";
    addRepoId(db, "69", "rhel");
}

void testHasProductId(dbFixture *fixture, gconstpointer ignored) {
    ProductDb *db = fixture->db;
    db->path = "testing";
    addRepoId(db, "69", "rhel");
    g_assert_true(hasProductId(db, "69"));
    g_assert_false(hasProductId(db, "notPresentProdId"));
}

void testRemoveProductId(dbFixture *fixture, gconstpointer ignored) {
    ProductDb *db = fixture->db;
    db->path = "testing";
    addRepoId(db, "69", "rhel");
    g_assert_true(hasProductId(db, "69"));
    g_assert_true(removeProductId(db, "69"));
    g_assert_false(hasProductId(db, "69"));

    g_assert_false(removeProductId(db, "0"));
}

void testHasRepoId(dbFixture *fixture, gconstpointer ignored) {
    ProductDb *db = fixture->db;
    db->path = "testing";
    addRepoId(db, "69", "rhel");
    g_assert_true(hasRepoId(db, "69", "rhel"));
    g_assert_false(hasRepoId(db, "69", "notPresentRepoId"));
    g_assert_false(hasRepoId(db, "notPresentProdId", "rhel"));
}

void testRemoveRepoId(dbFixture *fixture, gconstpointer ignored) {
    ProductDb *db = fixture->db;
    db->path = "testing";
    addRepoId(db, "69", "rhel");
    addRepoId(db, "69", "jboss");
    g_assert_true(hasRepoId(db, "69", "rhel"));
    g_assert_true(removeRepoId(db, "69", "rhel"));
    g_assert_false(hasRepoId(db, "69", "rhel"));
    g_assert_true(hasRepoId(db, "69", "jboss"));

    g_assert_false(hasRepoId(db, "69", "notPresentRepoId"));

}


int main(int argc, char **argv) {
    g_test_init(&argc, &argv, NULL);
    g_test_add("/set1/test add", dbFixture, NULL, setup, testAdd, teardown);
    g_test_add("/set1/test has product id", dbFixture, NULL, setup, testHasProductId, teardown);
    g_test_add("/set1/test remove product id", dbFixture, NULL, setup, testRemoveProductId, teardown);
    g_test_add("/set1/test has repo id", dbFixture, NULL, setup, testHasRepoId, teardown);
    g_test_add("/set1/test remove repo id", dbFixture, NULL, setup, testRemoveRepoId, teardown);
    return g_test_run();
}
