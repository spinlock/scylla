/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Copyright (C) 2015 Cloudius Systems, Ltd.
 */

#pragma once

#include <unordered_set>
#include <vector>
#include "gms/inet_address.hh"

namespace locator {

using inet_address = gms::inet_address;

struct i_endpoint_snitch
{
    /**
     * returns a String repesenting the rack this endpoint belongs to
     */
    virtual sstring get_rack(inet_address endpoint) = 0;

    /**
     * returns a String representing the datacenter this endpoint belongs to
     */
    virtual sstring get_datacenter(inet_address endpoint) = 0;

    /**
     * returns a new <tt>List</tt> sorted by proximity to the given endpoint
     */
    virtual std::vector<inet_address> get_sorted_list_by_proximity(inet_address address, std::unordered_set<inet_address>& unsorted_address) = 0;

    /**
     * This method will sort the <tt>List</tt> by proximity to the given address.
     */
    virtual void sort_by_proximity(inet_address address, std::vector<inet_address>& addresses) = 0;

    /**
     * compares two endpoints in relation to the target endpoint, returning as Comparator.compare would
     */
    virtual int compare_endpoints(inet_address& target, inet_address& a1, inet_address& a2) = 0;

    /**
     * called after Gossiper instance exists immediately before it starts gossiping
     */
    virtual void gossiper_starting() = 0;

    /**
     * Returns whether for a range query doing a query against merged is likely
     * to be faster than 2 sequential queries, one against l1 followed by one against l2.
     */
    virtual const bool is_worth_merging_for_range_query(std::vector<inet_address>& merged, std::vector<inet_address>& l1, std::vector<inet_address>& l2) const = 0;

    virtual ~i_endpoint_snitch();
};

class abstract_endpoint_snitch : public i_endpoint_snitch {
public:
    /**
     * Sorts the <tt>Collection</tt> of node addresses by proximity to the given address
     * @param address the address to sort by proximity to
     * @param unsortedAddress the nodes to sort
     * @return a new sorted <tt>List</tt>
     */
    virtual std::vector<inet_address> get_sorted_list_by_proximity(inet_address address, std::unordered_set<inet_address>& unsorted_address) override;

    /**
     * Sorts the <tt>List</tt> of node addresses, in-place, by proximity to the given address
     * @param address the address to sort the proximity by
     * @param addresses the nodes to sort
     */
    virtual void sort_by_proximity(inet_address address, std::vector<inet_address>& addresses) override;

    virtual void gossiper_starting() override;

    virtual const bool is_worth_merging_for_range_query(std::vector<inet_address>& merged, std::vector<inet_address>& l1, std::vector<inet_address>& l2) const override;

private:
    const bool has_remote_node(std::vector<inet_address>& l) const;
};

}
