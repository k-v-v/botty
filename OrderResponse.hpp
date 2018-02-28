//
// Created by konstantin on 27/02/18.
//

#pragma once

struct OrderResponse
{
    enum status_type{order_successful, order_unsuccessful} status;
    double amount;
    //Todo add anything else
};
