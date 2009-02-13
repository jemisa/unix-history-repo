/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#pragma D option quiet

BEGIN
{
	@bop[345] = quantize(0, 0);
	@baz[345] = lquantize(0, -10, 10, 1, 0);
}

BEGIN
{
	@foo[123] = sum(123);
	@bar[456] = sum(456);

	@foo[789] = sum(789);
	@bar[789] = sum(789);

	printa("%10d %@10d %@10d\n", @foo, @bar);
	printa("%10d %@10d %@10d %@10d\n", @foo, @bar, @bop);
	printa("%10d %@10d %@10d %@10d %@10d\n", @foo, @bar, @bop, @baz);

	exit(0);
}
