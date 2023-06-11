<script lang="ts">
	import BasePlot from './BasePlot.svelte';
	import { start, end } from '../../store/store';
	import { getContoursData } from './data.svelte';

	let startTime = '';
	let endTime = '';
	let trace: Plotly.Data;
	start.subscribe((time) => (startTime = time));
	end.subscribe((time) => (endTime = time));

    $: getTrace(startTime, endTime);

    async function getTrace(startTime: string, endTime: string) {
        const data = await getContoursData(startTime, endTime);
        trace = {
			x: data.contourAreas.flat(),
			type: 'histogram'
		}
    }
</script>

{#if trace}
	<BasePlot
		trace={trace}
		title="Contour Areas"
		xaxisTitle="Area"
		yaxisTitle="Amount"
	/>
{/if}
